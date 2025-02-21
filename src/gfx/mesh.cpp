#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/proper/error.hpp"
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: vao_hash(VAOManager::get_instance().compute_hash(vertices, indices)),
	  indices_length(static_cast<uint32>(indices.size())) {

	this->vao_id = VAOManager::get_instance().make_vao(vertices, indices);
}

Mesh::Mesh(const char* path) {
	// Data containers
	tinyobj::attrib_t attrib; // Mesh information
	std::vector<tinyobj::shape_t> shapes; // Mesh shapes
	std::vector<tinyobj::material_t> materials; // Mesh materials

	// Load obj
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

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

	this->vertices.reserve(total_indices);
	indices.reserve(total_indices);

	// Decompress
	std::unordered_map<Vertex, uint32_t> uniq_vertices;
	for(const tinyobj::shape_t& shape : shapes) {
		for(const tinyobj::index_t& index : shape.mesh.indices) {
			// Check if vertex is not negative and is not out of bounds
			if(index.vertex_index < 0 || static_cast<size_t>(index.vertex_index * 3 + 2) >= attrib.vertices.size()) {
				throw ScarabError("Invalid vertex index (%d)", index.vertex_index);
			}

			// WARNING: Dont push normal by now

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
					attrib.texcoords[static_cast<size_t>(index.texcoord_index * 2 + 1)],
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
				this->vertices.push_back(vertex);
			}
			indices.push_back(uniq_vertices[vertex]);
		}
	}

	this->indices_length = indices.size();
	this->vao_hash = VAOManager::get_instance().compute_hash(vertices, indices);
	this->vao_id = VAOManager::get_instance().make_vao(vertices, indices);
}


Mesh::~Mesh() noexcept {
	// Release current vao
	VAOManager::get_instance().release_vao(vao_hash);
}


void Mesh::set_texture(Texture* texture) noexcept {
	if(texture == nullptr){
		this->texture = &this->get_deftex();
		return;
	}

	this->texture = texture;
}

