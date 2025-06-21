#include "scarablib/scenes/Iscene.hpp"

IScene::IScene(Camera& camera) noexcept : camera(camera) {}

IScene::~IScene() noexcept {
	this->scene.clear();
	this->vao_groups.clear();
}


void IScene::remove_by_key(const std::string& key) {
	auto it = this->scene.find(key);
	if(it == this->scene.end()) {
		throw ScarabError("Key '%s' was not found", key.data());
	}

	// Remove from the scene key map
	this->scene.erase(key);
	// Remove from the scene vao map
	this->vao_groups.erase(it->second->bundle.vao->get_id());
}

void IScene::draw(Mesh& model) const noexcept {
	this->shader->use();
	model.bundle.vao->bind();
	model.material.texture->bind();
	model.draw_logic(this->camera,
		(model.get_shader() != nullptr) ? *model.get_shader() : *this->shader
	);
	model.material.texture->unbind();
	this->shader->unbind();
}

void IScene::draw_all() const noexcept {
	// Cache shader and camera
	Shader* shader = this->shader;
	Camera& camera = this->camera;

	// Track currently bound objects to avoid redundant bindings
	Shader* cur_shader   = this->shader;
	Texture* cur_texture = &Texture::default_texture();
	GLuint cur_vao       = 0;
	// GLenum texture_type = GL_TEXTURE_2D;

	for(const auto& [vao, models] : this->vao_groups) {
		// Bind VAO if its a different from the currently bound
		if(vao != cur_vao) {
			glBindVertexArray(vao);
			cur_vao = vao;
		}

		// Draw all models with this VAO
		for(const std::shared_ptr<Mesh>& model : models) {
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
				cur_texture->unbind();
				cur_texture = model->material.texture;
				cur_texture->bind();
			}

			// Draw model
			model->draw_logic(camera, *cur_shader);
		}
	}

	cur_shader->unbind();
	cur_texture->unbind();
	glBindVertexArray(0);
}

