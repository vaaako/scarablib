#include "scarablib/geometry/model.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/utils/model.hpp"

Model::Model() noexcept {
	this->material->shader = ResourcesManager::get_instance().load_shader_program({
		{ .source = Shaders::DEFAULT_VERTEX,   .type = Shader::Type::Vertex },
		{ .source = Shaders::DEFAULT_FRAGMENT, .type = Shader::Type::Fragment },
	});
}

Model::Model(const char* path) : Mesh() {
	// auto pair = ScarabModel::load_obj_old(path);
	// this->vertexarray = ResourcesManager::get_instance()
	// 	.acquire_vertexarray(pair.first, pair.second);
	// // Position and TexUV
	// this->vertexarray->add_attribute<float>(3, false);
	// this->vertexarray->add_attribute<float>(2, true);

	this->material->shader = ResourcesManager::get_instance().load_shader_program({
		{ .source = Shaders::DEFAULT_VERTEX,   .type = Shader::Type::Vertex },
		{ .source = Shaders::DEFAULT_FRAGMENT, .type = Shader::Type::Fragment },
	});

	auto pair = ScarabModel::load_obj(path);
	this->submeshes   = pair.first;
	this->vertexarray = pair.second;
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
	this->model = glm::translate(this->model, this->position.get());
	// Calculate rotation matrices
	this->model = glm::rotate(this->model, glm::radians(this->orient_angle), this->orient_axis);
	this->model = glm::rotate(this->model, glm::radians(this->angle), this->axis);
	// Scale
	this->model = glm::scale(this->model, this->scale.get());

	this->isdirty = false;

	// Update the bounding box in world space
	if(this->dynamic_bounding && this->bbox != nullptr) {
		this->bbox->update_world_bounds(this->model);
	}
}

// I just need to provide the mvp just if any of the matrix changes, because the value is stored
// but i dont know how to do it currently (and i am lazy)
void Model::draw_logic() noexcept {
	// Update the bounding box in world space
	if(this->dynamic_bounding && this->bbox != nullptr) {
		this->bbox->update_world_bounds(this->model);
	}

	if(this->submeshes.empty()) {
		glDrawElements(GL_TRIANGLES, this->vertexarray->get_length(), this->vertexarray->get_indices_type(), (void*)0);
		return;
	}

	// Iterate and draw each submesh
	uint32 last_texid = 0;
	for(const SubMesh& submesh : this->submeshes) {
		if(submesh.textureid != 0) {
			uint32 curid = submesh.textureid;
			if(curid != last_texid) {
			#if !defined(BUILD_OPGL30)
				glBindTextureUnit(0, curid);
			#else
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, curid);
			#endif
				last_texid = curid;
			}
		} else {
			Assets::default_texture()->bind();
		}

		// Draw only the portion of the EBO belonging to this submesh
		// Offset must be: index * sizeof(uint32)
		glDrawElements(
			GL_TRIANGLES,
			submesh.indices_count,
			this->vertexarray->get_indices_type(),
			this->vertexarray->index_offset(submesh.base_index)
		);
	}
}






/*
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
*/
