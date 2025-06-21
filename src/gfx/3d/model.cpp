#include "scarablib/gfx/3d/model.hpp"

Model::Model(const char* path) noexcept : Mesh(path) {}

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
	if(this->dynamic_bounding) {
		this->bbox.update_world_bounds(this->model);
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

