#include "scarablib/shapes/2d/circle.hpp"

Circle::Circle(const Shape2DConf& conf)
	: Shape2D(conf) {}

void Circle::draw(const Shader& shader) {
	shader.unbind();

	// Apply transformations if needed
	this->update_model();

	// Bind shader and apply changes
	Shader& circle_shader = this->get_circle_shader();
	circle_shader.use();
	circle_shader.set_matrix4f("model", this->model);
	circle_shader.set_vector4f("shapeColor", this->color.to_vec4<float>());
	circle_shader.set_float("blur", this->blur);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	this->texture->unbind();
}
