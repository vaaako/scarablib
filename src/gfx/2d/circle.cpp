#include "scarablib/gfx/2d/circle.hpp"
#include "scarablib/geometry/geometry_factory.hpp"

Circle::Circle() noexcept
	: Sprite(GeometryFactory::make_rectangle_vertices()) {}

void Circle::draw_logic(const Camera& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);
	shader.set_color("shapeColor", this->material.color);

	shader.set_float("blur", this->blur);

	// hard coded vertices size
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
