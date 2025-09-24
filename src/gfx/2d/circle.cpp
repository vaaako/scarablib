#include "scarablib/gfx/2d/circle.hpp"
#include "scarablib/geometry/geometry_factory.hpp"

Circle::Circle() noexcept
	: Sprite(GeometryFactory::make_rectangle_vertices()) {

	// Store and get one time only (deleted inside window destructor)
	this->material->shader = ShaderManager::get_instance().load_shader(
		"circle",
		Shaders::DEFAULT_VERTEX,
		Shaders::CIRCLE_FRAGMENT
	);
	// this->material->shader.set_custom("code here...");
}

void Circle::draw_logic(const Camera& camera) noexcept {
	this->update_model_matrix();

	std::shared_ptr<ShaderProgram> shader = this->material->shader; // cache
	shader->set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);
	shader->set_float("blur", this->blur);

	// hard coded vertices size
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
