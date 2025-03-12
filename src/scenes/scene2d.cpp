#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/gfx/2d/sprite.hpp"
#include <cstdio>
#include <memory>

// No need to set width and height here, since it will be set in update_viewport
Scene2D::Scene2D(Camera2D& camera) noexcept
	: IScene<Sprite>(), camera(camera) {}

void Scene2D::draw_all() const noexcept {
	// Cache shader and camera
	Shader* shader   = this->shader;
	Camera2D& camera = this->camera;

	// Track current shader to avoid redundant bindings
	Shader* cur_shader = this->shader;

	shader->use();

	// glDepthFunc(GL_ALWAYS);

	for(const auto& [vao, models] : this->vao_groups) {
		glBindVertexArray(vao);

		// Draw all sprites with this VAO
		for(std::shared_ptr<Sprite> sprite : models) {
			// Determine which shader to use
			Shader* sprite_shader = (sprite->get_shader() != nullptr) ? sprite->get_shader() : shader;

			// Change between default shader and different shader
			if(sprite_shader != cur_shader) {
				cur_shader->unbind();
				sprite_shader->use();
				cur_shader = sprite_shader;
			}

			// Draw sprite
			sprite->draw(camera, *cur_shader);
		}
	}

	// glDepthFunc(GL_LESS);

	glBindVertexArray(0);
	shader->unbind();
}


