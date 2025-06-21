#include "scarablib/scenes/scene3d.hpp"
#include <GL/glew.h>

Scene3D::Scene3D(Camera& camera) noexcept
	: IScene(), camera(camera) {}


void Scene3D::draw(Model& model) const noexcept {
	this->shader->use();
	model.bundle.vao->bind();
	model.draw_logic(this->camera,
		(model.get_shader() != nullptr) ? *model.get_shader() : *this->shader
	);
	this->shader->unbind();
}


void Scene3D::draw_all() const noexcept {
	// Cache shader and camera
	Shader* shader = this->shader;
	Camera& camera = this->camera;

	// Track currently bound objects to avoid redundant bindings
	Shader* cur_shader   = this->shader;
	Texture* cur_texture = &Texture::default_texture();
	GLuint cur_vao       = 0;
	// GLenum texture_type = GL_TEXTURE_2D;

	for(const auto& [vao, models] : this->vao_groups) {
		// Only bind VAO if it's different from the currently bound one
		if(vao != cur_vao) {
			glBindVertexArray(vao);
			cur_vao = vao;
		}

		// Draw all models with this VAO
		for(const std::shared_ptr<Model>& model : models) {
			// Determine which shader to use
			Shader* model_shader = (model->get_shader() != nullptr) ? model->get_shader() : shader;
			// Change between default shader and different shader
			if(model_shader != cur_shader) {
				cur_shader->unbind();
				model_shader->use();
				cur_shader = model_shader;
			}

			// Change texture if it's different from the currently bound one
			if(model->material.texture->get_id() != cur_texture->get_id()) {
				cur_texture = model->material.texture;
				cur_texture->bind();
			}

			// Draw model
			model->draw_logic(camera, *cur_shader);
		}
	}

	shader->unbind();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

