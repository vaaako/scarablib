#include "scarablib/utils/model.hpp"
#include "scarablib/geometry/submesh.hpp"
#include "scarablib/opengl/assets.hpp"
#include "scarablib/opengl/resourcesmanager.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/file.hpp"
#include <unordered_set>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>


std::pair<std::vector<SubMesh>, std::shared_ptr<VertexArray>> ScarabModel::load_obj(const char* path) {
	// Data containers
	tinyobj::attrib_t attrib;                   // Mesh information
	std::vector<tinyobj::shape_t> shapes;       // Mesh shapes
	std::vector<tinyobj::material_t> materials; // Mesh materials

	std::string err;
	std::filesystem::path modeldir = (ScarabFile::parent_dir(path).string() + "/");

	// Load obj
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
		path, // File
		modeldir.c_str(), // Base dir
		true // Triangulate
	)) {
		throw ScarabError("Failed to load/parse (%s) file: %s", path, err.c_str());
	}

	if(!err.empty()) {
		LOG_WARNING(err.c_str());
	}

	// Collect unique texture names to avoid checking the same file multiple times
	std::unordered_set<std::string> required_textures;
	for(const tinyobj::material_t& mat : materials) {
		if(!mat.diffuse_texname.empty()) {
			required_textures.insert(mat.diffuse_texname);
		}

		if(!mat.specular_texname.empty()) {
			required_textures.insert(mat.specular_texname);
		}

		if(!mat.bump_texname.empty()) {
			required_textures.insert(mat.bump_texname);
		}

		if(!mat.normal_texname.empty()) {
			required_textures.insert(mat.normal_texname);
		}
	}

	// Check if all materials are present
	for(const std::string& texname : required_textures) {
		std::filesystem::path texpath = modeldir / ScarabModel::treat_texname(texname);

		if(!ScarabFile::file_exists(texpath)) {
			throw ScarabError(
				"Missing texture '%s' referenced by model '%s'",
				texpath.c_str(),
				path
			);
		}
	}

	// Temporary storage to group data by material index before flattening
	struct RawSubMesh {
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
		std::unordered_map<Vertex, uint32> uniq_verts;
	};

	// Used to group same matid
	// int because tinyobj uses -1 for "no material"
	std::unordered_map<int, RawSubMesh> material_groups;
	for(const tinyobj::shape_t& shape : shapes) {
		size_t indexoffset = 0;

		// OBJ assigns materials per face
		for(size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			int matid = shape.mesh.material_ids[f];      // Which material this face uses
			int fv    = shape.mesh.num_face_vertices[f]; // Number of vertices in this face

			// Group all faces using matid
			// matid is -1 if no material is found, use as default bucket
			RawSubMesh& group = material_groups[matid];

			for(int v = 0; v < fv; v++) {
				// Iterate the vertices of the current face
				const tinyobj::index_t& index =
					shape.mesh.indices[indexoffset + v];

				Vertex vertex{};

				// Position
				vertex.position = {
						attrib.vertices[index.vertex_index * 3],
						attrib.vertices[index.vertex_index * 3 + 1],
						attrib.vertices[index.vertex_index * 3 + 2]
				};

				// TexUV
				if(index.texcoord_index >= 0) {
					vertex.texuv = {
						attrib.texcoords[index.texcoord_index * 2],
						attrib.texcoords[index.texcoord_index * 2 + 1]
						// 1.0f - attrib.texcoords[index.texcoord_index * 2 + 1] // Optimization: V-Flip
					};
				}

				// Deduplication scope is the material group
				// Push unique vertices only
				if(group.uniq_verts.find(vertex) == group.uniq_verts.end()) {
					group.uniq_verts[vertex] = static_cast<uint32>(group.vertices.size());
					group.vertices.push_back(vertex);
				}
				group.indices.push_back(group.uniq_verts[vertex]);
			}
			// Move to next face
			indexoffset += fv;
		}
	}

	// -- FLATTERNING: Put everything into one big buffer
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	std::vector<SubMesh> output;

	// Reserve memory to avoid reallocations
	size_t rv = 0;
	size_t ri = 0;
	for(auto& [id, g] : material_groups) {
		rv += g.vertices.size();
		ri += g.indices.size();
	}
	vertices.reserve(rv);
	indices.reserve(ri);

	for(auto& [matid, group] : material_groups) {
		SubMesh submesh;

		// Set up drawing parameters
		submesh.base_index    = static_cast<uint32>(indices.size()); // Start point in EBO
		submesh.indices_count = static_cast<uint32>(group.indices.size());
		uint32 vertex_offset  = static_cast<uint32>(vertices.size());

		// Load textures
		if(matid >= 0 && matid < (int)materials.size()) {
			const tinyobj::material_t& mat = materials[matid];
			if(!mat.diffuse_texname.empty()) {
				submesh.textureid = Assets::load(
					(modeldir / ScarabModel::treat_texname(mat.diffuse_texname)).c_str()
				)->get_id();
			}
		}

		// Append indices
		vertices.insert(vertices.end(), group.vertices.begin(), group.vertices.end());
		for(uint32 idx : group.indices) {
			indices.push_back(vertex_offset + idx);
		}
		output.push_back(submesh);
	}

	std::shared_ptr<VertexArray> vertexarray = ResourcesManager::get_instance()
		.acquire_vertexarray(vertices, indices);
	// Position and TexUV
	vertexarray->add_attribute<float>(3, false);
	vertexarray->add_attribute<float>(2, false);

	return {
		output,
		vertexarray
	};
}

std::pair<std::vector<Vertex>, std::vector<uint32>> ScarabModel::load_obj_old(const char* path) {
	// Data containers
	tinyobj::attrib_t attrib;                   // Mesh information
	std::vector<tinyobj::shape_t> shapes;       // Mesh shapes
	std::vector<tinyobj::material_t> materials; // Mesh materials

	std::string err;
	std::filesystem::path modeldir = (ScarabFile::parent_dir(path).string() + "/");

	// Load obj
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
		path, // File
		modeldir.c_str() // Base dir
	)) {
		throw ScarabError("Failed to load/parse (%s) file", path);
	}

	if(!err.empty()) {
		LOG_WARNING(err.c_str());
	}

	size_t total_indices = 0;
	for(const tinyobj::shape_t& shape : shapes) {
		total_indices += shape.mesh.indices.size();
	}

	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
	vertices.reserve(total_indices);
	indices.reserve(total_indices);

	// Decompress
	std::unordered_map<Vertex, uint32_t> uniq_vertices;
	for(const tinyobj::shape_t& shape : shapes) {
		for(const tinyobj::index_t& index : shape.mesh.indices) {
			// Check if vertex is not negative and is not out of bounds
			if(index.vertex_index < 0 || static_cast<size_t>(index.vertex_index * 3 + 2) >= attrib.vertices.size()) {
				throw ScarabError("Invalid vertex index (%d)", index.vertex_index);
			}

			// WARNING: Dont push normals yet
			// vec3<float> normal;
			// if(index.normal_index > 0) {
			// 	normal = {
			// 		attrib.normals[static_cast<size_t>(index.normal_index * 3)],
			// 		attrib.normals[static_cast<size_t>(index.normal_index * 3 + 1)],
			// 		attrib.normals[static_cast<size_t>(index.normal_index * 3 + 2)]
			// 	};
			// }

			vec2<float> texuv = { 0.0f, 0.0f };
			// Check if is valid and inside range
			if(index.texcoord_index >= 0 && static_cast<size_t>(index.texcoord_index * 2 + 1) < attrib.texcoords.size()) {
				texuv = {
					attrib.texcoords[static_cast<size_t>(index.texcoord_index * 2)],
					attrib.texcoords[static_cast<size_t>(index.texcoord_index * 2 + 1)]
				};
			}

			Vertex vertex = {
				.position = {
					attrib.vertices[static_cast<size_t>(index.vertex_index * 3)],
					attrib.vertices[static_cast<size_t>(index.vertex_index * 3 + 1)],
					attrib.vertices[static_cast<size_t>(index.vertex_index * 3 + 2)],
				},

				.texuv = texuv,
				// .normal = normal
			};

			// Push unique vertices only
			if(uniq_vertices.find(vertex) == uniq_vertices.end()) {
				uniq_vertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniq_vertices[vertex]);
		}
	}

	attrib.vertices.clear();
	attrib.normals.clear();
	attrib.texcoords.clear();
	shapes.clear();
	materials.clear();

	return {
		vertices,
		indices
	};
}
