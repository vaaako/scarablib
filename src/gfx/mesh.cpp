#include "scarablib/gfx/mesh.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/utils/string.hpp"
#include <algorithm>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>


// Used to convert vectors after loading model
template <typename OUT, typename INP>
std::vector<OUT> convert_to(const std::vector<INP>& indices) {
	std::vector<OUT> output;
	output.reserve(indices.size());
	for (INP val : indices) {
		output.push_back(static_cast<OUT>(val));
	}
	return output;
}

// Indices attributes are not needed to set here
// I will not make Bounding Box here because this is probably a 2D model
Mesh::Mesh(const std::vector<Vertex>& vertices) noexcept {
	this->bundle.make_vao(vertices, std::vector<uint8> {});

	// Is not indices, but will have a similar use
	this->indices_length = static_cast<GLsizei>(vertices.size());
}

Mesh::Mesh(const char* path) {
	// Data containers
	tinyobj::attrib_t attrib; // Mesh information
	std::vector<tinyobj::shape_t> shapes; // Mesh shapes
	std::vector<tinyobj::material_t> materials; // Mesh materials

	// Load obj
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path, (StringHelper::base_dir(path) + "/").c_str());

	if(!err.empty()) {
		throw ScarabError("Error loading obj (%s): %s", path, err.c_str());
	}

	if(!ret) {
		throw ScarabError("Failed to load/parse (%s) file", path);
	}

	// Make and Reserve space
	std::vector<uint32> indices;
	size_t total_indices = 0;
	for(const tinyobj::shape_t& shape : shapes) {
		total_indices += shape.mesh.indices.size();
	}

	std::vector<Vertex> vertices;
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

	// I think this will never be the case
	if(indices.empty()) {
		throw ScarabError("Failed to load/parse (%s) file. Indices are empty", path);
	}

	this->bbox = BoundingBox(vertices);
	this->indices_length = static_cast<GLsizei>(indices.size());

	// Check which indice type fits
	// Then convert vector to that type and upload to VAO
	const uint32 max_val = *std::max_element(indices.begin(), indices.end());
	if(max_val <= UINT8_MAX) {
		this->indices_type = GL_UNSIGNED_BYTE;
		this->bundle.make_vao(vertices, convert_to<uint8>(indices));

	} else if(max_val <= UINT16_MAX) {
		this->indices_type = GL_UNSIGNED_SHORT;
		this->bundle.make_vao(vertices, convert_to<uint16>(indices));

	// Is uint32 or uint64, use existing indices
	} else {
		this->indices_type = GL_UNSIGNED_INT;
		this->bundle.make_vao(vertices, indices);
	}

	// Free memory
	attrib.vertices.clear();
	attrib.normals.clear();
	attrib.texcoords.clear();
	shapes.clear();
	materials.clear();
	// Actually releases the allocated memory
	attrib.vertices.shrink_to_fit();
	attrib.normals.shrink_to_fit();
	attrib.texcoords.shrink_to_fit();
	shapes.shrink_to_fit();
	materials.shrink_to_fit();
}


Mesh::~Mesh() noexcept {
	delete this->physics;
}


void Mesh::enable_physics(const float mass, const bool isstatic) noexcept {
	this->physics = new PhysicsComponent();
	this->physics->mass = mass;
	this->physics->isstatic = isstatic;
}
