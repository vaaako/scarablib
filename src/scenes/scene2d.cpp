#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/gfx/2d/sprite.hpp"
#include <cstdio>
#include <memory>

// No need to set width and height here, since it will be set in update_viewport
Scene2D::Scene2D(Camera2D& camera) noexcept
	: IScene<Sprite>(), camera(camera) {}

Scene2D::~Scene2D() noexcept {
	delete this->shader;
}

void Scene2D::add_to_scene(const char* key, Sprite* shape) {
	if(!shape) {
		throw ScarabError("Attempted to add a null Shape to the scene with key '%s'", key);
	}

	std::shared_ptr<Sprite> shared_shape = std::shared_ptr<Sprite>(shape);
	this->scene.emplace(key, shared_shape); // will not be used here, but is used for get_by_key()
	this->vao_groups[shape->get_vaoid()].emplace_back(shared_shape);
}

void Scene2D::draw_all() const noexcept {
	// Cache shader and camera
	Shader* shader   = this->shader;
	Camera2D& camera = this->camera;

	// This is just necessary if doing 2D and 3D
	glDepthFunc(GL_ALWAYS);

	shader->use();

	for(const auto& [vao, models] : this->vao_groups) {
		glBindVertexArray(vao);

		// Draw all sprites with this VAO
		for(std::shared_ptr<Sprite> sprite : models) {
			Shader* sprite_shader = sprite->get_shader();
			if(sprite_shader != nullptr) {
				sprite_shader->use();
				// Pass deafult shader since this is a virtual method, and the shader wont be used inside anyway
				sprite->draw(camera, *sprite_shader);
				sprite_shader->unbind();

				shader->use(); // Rebind scene shader
				continue;
			}

			// Draw sprite
			sprite->draw(camera, *shader);
		}
	}

	glBindVertexArray(0);
	shader->unbind();

	glDepthFunc(GL_LESS);
}


