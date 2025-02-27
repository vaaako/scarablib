#include "scarablib/gfx/2d/circle.hpp"

Circle::Circle(const std::vector<Vertex>& vertices) noexcept
	: Sprite(vertices) {}

void Circle::draw(const Camera2D& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);
	shader.set_color("shapeColor", this->color);

	shader.set_float("blur", this->blur);

	this->texture->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	this->texture->unbind();
}
