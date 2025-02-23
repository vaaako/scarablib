#include "scarablib/gfx/model.hpp"

Model::Model(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Mesh(vertices, indices) {}

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

	// Update the bounding box in world space
	if(this->calc_enabled) {
		this->bounding->update_world_bounding_box(this->model);
	}

	this->isdirty = false;
}

void Model::draw(const Camera& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	glm::mat4 view = camera.get_view_matrix();
	glm::mat4 proj = camera.get_proj_matrix(); // Add perspective

	// NOTE: "is dirty" for color wouldn't work because would set the last color updated for all Modeles (using this later maybe)
	shader.set_color("shapeColor", this->color);
	shader.set_matrix4f("mvp", (proj * view) * this->model);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);
	this->texture->unbind();
}
