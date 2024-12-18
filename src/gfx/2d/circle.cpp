#include "scarablib/gfx/2d/circle.hpp"

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
	shader.set_color("shapeColor", this->conf.color);
	circle_shader.set_float("blur", this->blur);

	this->texture->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	this->texture->unbind();
}
