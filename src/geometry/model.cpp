#include "scarablib/geometry/model.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/string.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

Model::Model(const char* path) : Mesh() {
	std::vector<uint32> indices;
	std::vector<Vertex> vertices = Model::load_obj(path, &indices);

	// I think this will never be the case
	if(indices.empty()) {
		throw ScarabError("Failed to load/parse (%s) file. Indices are empty", path);
	}

	// Create Vertex Array Buffer
	this->vertexarray = ResourcesManager::get_instance()
		.acquire_vertexarray(vertices, indices);
	// Position and TexUV
	this->vertexarray->add_attribute<float>(3, false);
	this->vertexarray->add_attribute<float>(2, true);

	// Set shader
	// NOTE: Redundant because the same is set inside MaterialCompoment
	std::shared_ptr<ShaderProgram> shader = ResourcesManager::get_instance().load_shader_program({
		// Default vertex and fragment shader source
		{ .source = Shaders::DEFAULT_VERTEX,   .type = Shader::Type::Vertex },
		{ .source = Shaders::DEFAULT_FRAGMENT, .type = Shader::Type::Fragment },
	});
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
void Model::draw_logic(const Camera& camera) noexcept {
	// Shader is binded outside for batch rendering
	this->update_model_matrix();

	// NOTE: is_dirty for color wouldn't work because would set this color to the next meshes
	this->material->shader->set_matrix4f("mvp",
		(camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);
	glDrawElements(GL_TRIANGLES, this->vertexarray->get_length(), this->vertexarray->get_indices_type(), (void*)0);
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



std::vector<MeshTriangle> Model::get_obj_triangles(const char* path, const glm::mat4& transform) {
	std::vector<MeshTriangle> triangles;

	// Data containers
	tinyobj::attrib_t attrib;                   // Mesh information
	std::vector<tinyobj::shape_t> shapes;       // Mesh shapes
	std::vector<tinyobj::material_t> materials; // Mesh materials

	// Load obj
	std::string warn, err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path, (StringHelper::base_dir(path) + "/").c_str());

	// TODO: WARNINGS are also displayed here, throw just if error
	if(!err.empty()) {
		LOG_WARNING(err.c_str());
	}

	if(!ret) {
		throw ScarabError("Failed to load/parse (%s) file", path);
	}

	// Loop trough each shape in the file
	for(const tinyobj::shape_t& shape : shapes) {
		// Loop through each index of a face
		// The indices are grouped by 3, because we assume the model is triangulated.
		for(size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
			// Get the three vertices that make up this one triangle
			tinyobj::index_t idx0 = shape.mesh.indices[i + 0];
			tinyobj::index_t idx1 = shape.mesh.indices[i + 1];
			tinyobj::index_t idx2 = shape.mesh.indices[i + 2];

			// Extract the position for each vertex from the attrib list
			// The attrib.vertices is a flat array [x0, y0, z0, x1, y1, z1, ...]
			vec3<float> v0 = vec3<float>(
				attrib.vertices[3 * (size_t)idx0.vertex_index + 0],
				attrib.vertices[3 * (size_t)idx0.vertex_index + 1],
				attrib.vertices[3 * (size_t)idx0.vertex_index + 2]
			);

			vec3<float> v1 = vec3<float>(
				attrib.vertices[3 * (size_t)idx1.vertex_index + 0],
				attrib.vertices[3 * (size_t)idx1.vertex_index + 1],
				attrib.vertices[3 * (size_t)idx1.vertex_index + 2]
			);

			vec3<float> v2 = vec3<float>(
				attrib.vertices[3 * (size_t)idx2.vertex_index + 0],
				attrib.vertices[3 * (size_t)idx2.vertex_index + 1],
				attrib.vertices[3 * (size_t)idx2.vertex_index + 2]
			);

			// Apply the transformation to the vertices
			MeshTriangle tri;
			tri.v0 = vec3<float>(transform * vec4<float>(v0, 1.0f));
			tri.v1 = vec3<float>(transform * vec4<float>(v1, 1.0f));
			tri.v2 = vec3<float>(transform * vec4<float>(v2, 1.0f));

			// Calculate the triangle's normal
			tri.normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

			triangles.push_back(tri);
		}
	}

	attrib.vertices.clear();
	attrib.normals.clear();
	attrib.texcoords.clear();
	shapes.clear();
	materials.clear();

	return triangles;
}
