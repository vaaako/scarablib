#include "scarablib/shapes/2d/circle.hpp"

Circle::Circle(const vec2<float>& position, const vec2<float>& size, const Color& color, const float angle)
	: Shape2D(position, size, color, angle) {}

void Circle::draw(const Shader& shader) {
	// Apply transformations if needed
	this->update_model();

	shader.set_matrix4f("model", this->model);
	shader.set_vector4f("shapeColor", this->color.to_vec4<float>());

	shader.set_float("blur", this->blur);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	this->texture->unbind();
}
