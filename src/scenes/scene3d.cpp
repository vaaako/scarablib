#include "scarablib/scenes/scene3d.hpp"
#include <GL/glew.h>
#include <algorithm>

Scene3D::Scene3D(Camera& camera) noexcept : IScene(), camera(camera) {}

Scene3D::~Scene3D() noexcept {
	delete this->shader;
}

void Scene3D::add_to_scene(const std::string& key, Model* model) {
	if(!model) {
		throw ScarabError("Attempted to add a null Model to the scene with key '%s'", key.c_str());
	}

	std::shared_ptr<Model> shared_mesh = std::shared_ptr<Model>(model);
	this->scene.emplace(key, shared_mesh); // Used be get_by_key()

	auto& vao_groups = this->vao_groups[model->get_vaoid()];

	// Sort shaders to minimize shader changes
	// Find the correct position for insertion
	auto it = std::lower_bound(vao_groups.begin(), vao_groups.end(), shared_mesh,
		[](const std::shared_ptr<Model>& a, const std::shared_ptr<Model>& b) {

		// If a has no shader (default) and b has a shader, put a before b
		if(a->get_shader() == nullptr && b->get_shader() != nullptr) {
			return true;
		}

		// If a has a shader and b has no shader (default), put b before a
		if(a->get_shader() != nullptr && b->get_shader() == nullptr) {
			return false;
		}

		// Fallback: Sort by shader ID
		if(a->get_shader() != nullptr && b->get_shader() != nullptr) {
			return a->get_shader()->get_id() < b->get_shader()->get_id();
		}

		// Fallback: If both have no shaders, maintain original order
		return false;
	});

	// Insert the model in the correct place
	vao_groups.insert(it, shared_mesh);
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

			// Draw bounding box
			if((this->draw_every_box || model->draw_box) && model->bounding) {
				model->bounding->draw(camera, *shader);
				glBindVertexArray(vao); // Rebind the model's VAO (since bounding box unbinds VAO)
			}
		}
	}

	shader->unbind();
	glBindVertexArray(0);
}

