#include "scarablib/gfx/model.hpp"

Model::Model(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Mesh(vertices, indices), conf(conf), bounding(vertices) {
	// Compute local bounding box from vertices
	bounding.compute_bounding_box(vertices);

	this->vertices.clear();
}

Model::Model(const char* path) noexcept : Mesh(path), bounding(vertices) {
	// Compute local bounding box from vertices
	bounding.compute_bounding_box(vertices);

	// Hack, i dont like this, i want to think in a proper solution
	// i dont want to put boundingbox in mesh because dont fit mesh propose
	this->vertices.clear();
}

void Model::set_rotation(const float angle, const vec3<bool> axis) noexcept {
	this->conf.angle = angle;

	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return; // Dont update
	}

	// Just update if at least one true
	this->conf.axis = axis;
	this->isdirty = true;
}

void Model::set_orientation(const float angle, const vec3<bool> axis) noexcept {
	this->conf.orient_angle = angle;

	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return;
	}

	// Just update if at least one true
	this->conf.orient_axis = axis;
	this->isdirty = true;
}


void Model::update_model_matrix() noexcept {
	if(!this->isdirty) {
		return;
	}

	// Calculate rotation matrices
	const glm::mat4 rotation    = glm::rotate(glm::mat4(1.0f), glm::radians(this->conf.angle), this->conf.axis);
	const glm::mat4 orientation = glm::rotate(glm::mat4(1.0f), glm::radians(this->conf.orient_angle), this->conf.orient_axis);

	// Modify in place
	this->model = glm::translate(glm::mat4(1.0f), this->conf.position);
	this->model = glm::scale(this->model, this->conf.scale);
	this->model *= orientation;
	this->model *= rotation;

	// Update the bounding box in world space
	// Just if enabled (since will be only visible if enabled)
	if(this->collision_enabled) {
		this->bounding.update_world_bounding_box(this->model);
	}

	this->isdirty = false;
}

void Model::draw(const Camera& camera, const Shader& shader) noexcept {
	this->update_model_matrix();
	
	// View matrix
	glm::mat4 view = camera.get_view_matrix();
	// Add perspective
	glm::mat4 proj = camera.get_proj_matrix();

	// NOTE: "is dirty" for color wouldn't work because would set the last color updated for all Modeles (using this later maybe)
	shader.set_color("shapeColor", this->conf.color);
	shader.set_matrix4f("mvp", (proj * view) * this->model);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);
	this->texture->unbind();
}
