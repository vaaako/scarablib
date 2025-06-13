#include "scarablib/gfx/2d/circle.hpp"
#include "scarablib/gfx/geometry_factory.hpp"

Circle::Circle() noexcept
	: Sprite(GeometryFactory::make_rectangle_vertices()) {}

void Circle::draw(const Camera2D& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);
	shader.set_color("shapeColor", this->material.color);

	shader.set_float("blur", this->blur);

	this->material.texture->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	this->material.texture->unbind();
}
