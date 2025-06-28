#include "scarablib/geometry/sprite.hpp"
#include "scarablib/geometry/mesh.hpp"

Sprite::Sprite(const std::vector<Vertex>& vertices) noexcept
	: Mesh(vertices) {}

void Sprite::update_model_matrix() noexcept {
	// Just update if is dirty
	if(!this->isdirty) {
		return;
	}

	this->model = glm::mat4(1.0f);

	// Modify in place
	this->model = glm::translate(this->model, glm::vec3(static_cast<const glm::vec2&>(this->position), 0.0f));
	// Origin from top-lef to center, to apply rotation
	this->model = glm::translate(this->model, glm::vec3(0.5f * this->size->x, 0.5f * this->size->y, 0.0f));
	// Rotate
	this->model = glm::rotate(this->model, glm::radians(static_cast<float>(this->angle)), glm::vec3(0.0f, 0.0f, 1.0f));
	// Origin back to top-left
	this->model = glm::translate(this->model, glm::vec3(-0.5f * this->size->x, -0.5f * this->size->y, 0.0f));
	this->model = glm::scale(this->model, glm::vec3(static_cast<const glm::vec2&>(this->size), 0.0f));

	this->isdirty = false;

	// Collision calculation will be here
}

// I could just provide mvp if any of the matrix changes, because the value is stored in memory.
// But i just know when model changes, and camera matrix is likely to change everytime
void Sprite::draw_logic(const Camera& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	// NOTE: is_dirty for color wouldn't work because would set this color to the next meshes
	shader.set_color("shapeColor", this->material.color);
	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);

	// Not indices btw
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->indices_length);
}

