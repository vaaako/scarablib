#include "scarablib/scenes/scene3d.hpp"
#include <GL/glew.h>

Scene3D::Scene3D(Camera& camera) noexcept : IScene(), camera(camera) {}

Scene3D::~Scene3D() noexcept {
	delete this->shader;
}

void Scene3D::add_to_scene(const std::string& key, Model* model) {
	if(!model) {
		throw ScarabError("Attempted to add a null Model to the scene with key '%s'", key.c_str());
	}

	std::shared_ptr<Model> shared_mesh = std::shared_ptr<Model>(model);
	this->scene.emplace(key, shared_mesh); // will not be used here, but is used for get_by_key()
	this->vao_groups[model->get_vaoid()].emplace_back(shared_mesh);
}

void Scene3D::draw_all() const noexcept {
	// Cache shader and camera
	Shader* shader = this->shader;
	Camera& camera = this->camera;

	shader->use();

	for(const auto& [vao, models] : this->vao_groups) {
		glBindVertexArray(vao);

		// Draw all sprites with this VAO
		for(std::shared_ptr<Model> model : models) {
			if((this->draw_every_box || model->draw_box) && model->bounding) {
				model->bounding->draw(camera, *shader);
				glBindVertexArray(vao); // Rebind the model's VAO (since bounding box unbind VAO)
			}

			// Shader is not from model class
			Shader* model_shader = model->get_shader();
			if(model_shader != nullptr) {
				model_shader->use();
				// Pass deafult shader since this is a virtual method, and the shader wont be used inside anyway
				model->draw(camera, *model_shader);
				model_shader->unbind();

				shader->use(); // Rebind scene shader
				continue;
			}

			// Draw model
			model->draw(camera, *shader);
		}
	}

	glBindVertexArray(0);
	shader->unbind();
}

