#include "scarablib/scenes/scene3d.hpp"
#include <GL/glew.h>

Scene3D::Scene3D(Camera& camera) noexcept
	: IScene(), camera(camera) {}


void Scene3D::draw(Model& model) const noexcept {
	this->shader->use();
	model.get_bundle().bind_vao();
	model.draw(this->camera,
		(model.get_shader() != nullptr) ? *model.get_shader() : *this->shader
	);
	this->shader->unbind();
}

void Scene3D::draw_all() const noexcept {
	// Cache shader and camera
	Shader* shader = this->shader;
	Camera& camera = this->camera;

	// Track currently bound objects to avoid redundant bindings
	Shader* cur_shader = this->shader;
	GLuint cur_vao = 0;

	shader->use();

	for(const auto& [vao, models] : this->vao_groups) {
		// Only bind VAO if it's different from the currently bound one
		if(vao != cur_vao) {
			glBindVertexArray(vao);
			cur_vao = vao;
		}

		// Draw all sprites with this VAO
		for(std::shared_ptr<Model> model : models) {
			// Determine which shader to use
			Shader* model_shader = (model->get_shader() != nullptr) ? model->get_shader() : shader;

			// Change between default shader and different shader
			if(model_shader != cur_shader) {
				cur_shader->unbind();
				model_shader->use();
				cur_shader = model_shader;
			}

			// Draw model
			model->draw(camera, *cur_shader);
		}
	}

	shader->unbind();
	glBindVertexArray(0);
}

