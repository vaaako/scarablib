#include "scarablib/gfx/2d/rectangle.hpp"
#include <cstdio>

Rectangle::Rectangle(const Shape2DConf& conf)
	: Shape2D(conf) {}

void Rectangle::draw(const Shader& shader) {
	// Apply transformations if needed
	this->update_model();

	shader.set_matrix4f("model", this->model);
	shader.set_color("shapeColor", this->conf.color);

	// this->texture->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	this->texture->unbind();
}
