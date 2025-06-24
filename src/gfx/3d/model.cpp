#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/utils/string.hpp"
#include <algorithm>


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

Model::Model(const char* path) : Mesh() {
	std::vector<uint32> indices;
	std::vector<Vertex> vertices = Model::load_obj(path, &indices);

	// I think this will never be the case
	if(indices.empty()) {
		throw ScarabError("Failed to load/parse (%s) file. Indices are empty", path);
	}

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
	vertices.clear();
	indices.clear();
}

void Model::set_rotation(const float angle, const vec3<float>& axis) noexcept {
	// At least one axis need to be true to work
	if(axis == vec3<float>(0.0f)) {
		return; // Dont update
	}

	this->angle = angle;
	this->axis = axis;
	this->isdirty = true;
}

void Model::set_orientation(const float angle, const vec3<float>& axis) noexcept {
	// At least one axis need to be true to work
	if(axis == vec3<float>(0.0f)) {
		return;
	}

	this->orient_angle = angle;
	this->orient_axis = axis;
	this->isdirty = true;
}


void Model::update_model_matrix() noexcept {
	if(!this->isdirty) {
		return;
	}

	this->model = glm::mat4(1.0f);

	// Translate
	this->model = glm::translate(this->model, static_cast<const glm::vec3&>(this->position));
	// Calculate rotation matrices
	this->model = glm::rotate(this->model, glm::radians(this->orient_angle), this->orient_axis);
	this->model = glm::rotate(this->model, glm::radians(this->angle), this->axis);
	// Scale
	this->model = glm::scale(this->model, static_cast<const glm::vec3&>(this->scale));

	this->isdirty = false;

	// Update the bounding box in world space
	if(this->dynamic_bounding && this->bbox != nullptr) {
		this->bbox->update_world_bounds(this->model);
	}
}

// I just need to provide the mvp just if any of the matrix changes, because the value is stored
// but i dont know how to do it currently (and i am lazy)
void Model::draw_logic(const Camera& camera, const Shader& shader) noexcept {
	// Shader is binded outside for batch rendering
	this->update_model_matrix();

	// NOTE: is_dirty for color wouldn't work because would set this color to the next meshes
	shader.set_color("shapeColor", this->material.color);
	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);

	glDrawElements(GL_TRIANGLES, this->indices_length, this->indices_type, (void*)0);
}



std::vector<Vertex> Model::load_obj(const char* path, std::vector<uint32>* indices) {
	// Data containers
	tinyobj::attrib_t attrib;                   // Mesh information
	std::vector<tinyobj::shape_t> shapes;       // Mesh shapes
	std::vector<tinyobj::material_t> materials; // Mesh materials

	// Load obj
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path, (StringHelper::base_dir(path) + "/").c_str());

	// TODO: WARNINGS are also displayed here, throw just if error
	if(!err.empty()) {
		LOG_WARNING(err.c_str());
		// throw ScarabError("Error loading obj (%s): %s", path, err.c_str());
	}

	if(!ret) {
		throw ScarabError("Failed to load/parse (%s) file", path);
	}

	// Make and Reserve space
	size_t total_indices = 0;
	for(const tinyobj::shape_t& shape : shapes) {
		total_indices += shape.mesh.indices.size();
	}
	if(indices) {
		indices->reserve(total_indices);
	}

	std::vector<Vertex> vertices;
	vertices.reserve(total_indices);

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
			if(indices) {
				indices->push_back(uniq_vertices[vertex]);
			}
		}
	}

	attrib.vertices.clear();
	attrib.normals.clear();
	attrib.texcoords.clear();
	shapes.clear();
	materials.clear();

	return vertices;
}

