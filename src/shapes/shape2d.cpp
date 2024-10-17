#include "scarablib/shapes/shape2d.hpp"

Shape2D::Shape2D(const vec2<float>& position, const vec2<float>& size, const Color& color, const float angle)
	: position(position), size(size), color(color), angle(angle),
	  model(glm::mat4(1.0f)), isdirty(true) {}

void Shape2D::update_model() {
	// Use current texture
	this->texture->bind();

	// Just update if is dirty
	if(!this->isdirty) {
		return;
	}

	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, glm::vec3(this->position.x, this->position.y, 0.0f))
				* glm::translate(model, glm::vec3(0.5f * this->size.x, 0.5f * this->size.y, 0.0f))   // Origin from top-lef to center
				* glm::rotate(model, glm::radians(this->angle), glm::vec3(0.0f, 0.0f, 1.0f))         // Rotate
				* glm::translate(model, glm::vec3(-0.5f * this->size.x, -0.5f * this->size.y, 0.0f)) // Origin back to top-left
				* glm::scale(this->model, glm::vec3(this->size.x, this->size.y, 1.0f));
	this->isdirty = false;
}

