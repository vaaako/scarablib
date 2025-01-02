#include "scarablib/gfx/shape2d.hpp"

Shape2D::Shape2D(const Shape2DConf& conf)
	: conf(conf), model(glm::mat4(1.0f)), isdirty(true) {}

void Shape2D::update_model() {
	this->texture->bind();
	// Just update if is dirty
	if(!this->isdirty) {
		return;
	}

	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, glm::vec3(this->conf.position, 0.0f))
				* glm::translate(this->model, glm::vec3(0.5f * this->conf.size.x, 0.5f * this->conf.size.y, 0.0f))   // Origin from top-lef to center
				* glm::rotate(this->model, glm::radians(this->conf.angle), glm::vec3(0.0f, 0.0f, 1.0f))         // Rotate
				* glm::translate(this->model, glm::vec3(-0.5f * this->conf.size.x, -0.5f * this->conf.size.y, 0.0f)) // Origin back to top-left
				* glm::scale(this->model, glm::vec3(this->conf.size, 0.0f));
	this->isdirty = false;
}

