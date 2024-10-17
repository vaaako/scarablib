#include "scarablib/shapes/2d/rectangle.hpp"
#include <cstdio>

Rectangle::Rectangle(const vec2<float>& position, const vec2<float>& size, const Color& color, const float angle)
	: Shape2D(position, size, color, angle) {}

void Rectangle::draw(const Shader& shader) {
	// Apply transformations if needed
	this->update_model();

	shader.set_matrix4f("model", this->model);
	shader.set_vector4f("shapeColor", this->color.to_vec4<float>());
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

}
