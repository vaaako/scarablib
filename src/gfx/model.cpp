#include "scarablib/gfx/model.hpp"

Model::Model(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Mesh(vertices, indices), conf(conf), bounding(vertices) {
	// Compute local bounding box from vertices
	bounding.compute_bounding_box(this->vertices);
}

Model::Model(const char* path) noexcept : Mesh(path), bounding(vertices) {
	// Compute local bounding box from vertices
	bounding.compute_bounding_box(this->vertices);
}

Model& Model::set_rotation(const float angle, const vec3<bool> axis) noexcept {
	this->conf.angle = angle;

	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return *this; // Dont update
	}

	// Just update if at least one true
	this->conf.axis = (vec3<float>)axis;
	this->isdirty = true;
	return *this;
}

Model& Model::set_orientation(const float angle, const vec3<bool> axis) noexcept {
	this->conf.orient_angle = angle;

	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return *this;
	}

	// Just update if at least one true
	this->conf.orient_axis = (vec3<float>)axis;
	this->isdirty = true;
	return *this;
}

void Model::draw(const Camera& camera, const Shader& shader) noexcept {
	if(this->isdirty) {
		this->model = glm::mat4(1.0f);

		// Model matrix
		// vec3<float> center = this->center_pos();
		this->model = glm::translate(this->model, this->conf.position)
					// * glm::translate(this->model, center) // Move to origin
					* glm::scale(this->model, this->conf.scale)
					// * glm::translate(this->model, -center) // Move back
					* glm::rotate(this->model, glm::radians(this->conf.orient_angle), this->conf.orient_axis)
					* glm::rotate(this->model, glm::radians(this->conf.angle), this->conf.axis);

		// Update the bounding box in world space
		this->bounding.update_world_bounding_box(this->model);

		this->isdirty = false;
	}
	
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


