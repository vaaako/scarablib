#include "scarablib/gfx/3d/model.hpp"

Model::Model(const char* path) noexcept : Mesh(path) {}

void Model::set_rotation(const float angle, const vec3<bool>& axis) noexcept {
	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return; // Dont update
	}

	this->angle = angle;
	this->axis = axis;
	this->isdirty = true;
}

void Model::set_orientation(const float angle, const vec3<bool>& axis) noexcept {
	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
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

	// Calculate rotation matrices
	const glm::mat4 rotation    = glm::rotate(this->model, glm::radians(this->angle), this->axis);
	const glm::mat4 orientation = glm::rotate(this->model, glm::radians(this->orient_angle), this->orient_axis);

	// Modify in place
	this->model = glm::translate(this->model, this->position);
	this->model = glm::scale(this->model, this->scale);
	this->model *= orientation;
	this->model *= rotation;

	this->isdirty = false;

	// Update the bounding box in world space
	if(this->calc_enabled) {
		this->bounding->update_world_bounding_box(this->model);
	}
}

// I just need to provide the mvp just if any of the matrix changes, because the value is stored
// but i dont know how to do it currently (and i am lazy)
void Model::draw(const Camera& camera, const Shader& shader) noexcept {
	// Shader is binded outside for batch rendering

	this->update_model_matrix();

	// NOTE: "is dirty" for color wouldn't work because would set the last color updated for all Modeles (using this later maybe)
	shader.set_color("shapeColor", this->color);
	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, this->indices_length, this->indices_type, (void*)0);
	this->texture->unbind();
}
