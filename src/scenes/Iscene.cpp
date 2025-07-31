#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/gfx/color.hpp"
#include "scarablib/gfx/texture_array.hpp"

IScene::IScene(Camera& camera) noexcept : camera(camera) {}

IScene::~IScene() noexcept {
	this->scene.clear();
	this->vao_groups.clear();
}


void IScene::remove_by_key(const std::string& key) {
	if(!this->scene.contains(key)) {
		throw ScarabError("Key '%s' was not found", key.data());
	}

	// Remove from the scene vao map
	this->vao_groups.erase(this->scene.at(key)->bundle.vao->get_id());
	// Remove from the scene key map
	this->scene.erase(key);
}

// TODO: Not finished i need to check if its working and change a little bit
void IScene::draw(Mesh& model) const noexcept {
	this->shader->use();
	// Shader* shader = (model.get_shader() != nullptr) ? model.get_shader() : shader;

	shader->set_int("texSampler", 0);      // Bind texture to unit 0
	shader->set_int("texSamplerArray", 1); // Bind to unit 1

	model.bundle.vao->bind();

	const MaterialComponent::TextureHandle& texture = model.material->texture;
	const TextureArray* texture_array = model.material->texture_array;

	const bool hastex = model.material->texture != nullptr;
	const bool hastexarray = texture_array != nullptr;

	if(hastex) {
		texture->bind(0); // Unit 0
	}

	// Use texture array if it exists
	if(hastexarray) {
		texture_array->bind(1); // Unit 1
		shader->set_int("texlayer", texture_array->texture_index);
	}

	// Not need for cur_mixamount because this is irrelevant
	const float mix_amount = (hastex && hastexarray) ? model.material->mix_amount : (hastexarray ? 1.0f : 0.0f);
	shader->set_float("mixAmount", mix_amount);

	shader->set_color("shapeColor", model.material->color);

	model.draw_logic(this->camera,
		(model.get_shader() != nullptr) ? *model.get_shader() : *this->shader
	);
}

// glUseProgram      - Most expensive change
// glBindTexture     - Very expensive change
// glBindVertexArray - Less expensive change
// glUniform         - Cheapest change

void IScene::draw_all() const noexcept {
	// Cache shader and camera
	Shader* shader = this->shader;
	Camera& camera = this->camera;

	// Track currently bound objects to avoid redundant bindings
	GLuint cur_vao = 0;
	Shader* cur_shader = this->shader;
	MaterialComponent::TextureHandle cur_texture;
	TextureArray* cur_texarray = nullptr;
	Color cur_color = Colors::WHITE;
	// GLenum texture_type = GL_TEXTURE_2D;

	// Bind defaults
	cur_shader->use();   // Bind default shader
	cur_texture->bind(); // Bind default texture

	// Uniforms
	cur_shader->set_int("texSampler", 0);      // Bind texture to unit 0
	cur_shader->set_int("texSamplerArray", 1); // Bind to unit 1
	cur_shader->set_color("shapeColor", cur_color);

	for(const auto& [vao, models] : this->vao_groups) {
		// Bind VAO if its a different from the currently bound
		if(vao != cur_vao) {
			glBindVertexArray(vao);
			cur_vao = vao;
		}

		// Draw all models with this VAO
		for(const std::shared_ptr<Mesh>& model : models) {
			// -- SHADER //
			// Determine which shader to use
			Shader* model_shader = (model->get_shader() != nullptr) ? model->get_shader() : shader;
			// Change between default shader and different shader
			if(model_shader != cur_shader) {
				cur_shader->unbind();
				model_shader->use();
				cur_shader = model_shader;

				// Bind Again
				cur_shader->set_int("texSampler", 0);      // Bind texture to unit 0
				cur_shader->set_int("texSamplerArray", 1); // Bind to unit 1
				cur_shader->set_color("shapeColor", cur_color);
				// NOTE: Since shader probrably has texSamplerArray, i need to bind so mix function can work
			}
			// SHADER -- //


			// -- TEXTURE //
			const bool hastex = model->material->texture != nullptr;
			const bool hastexarray = model->material->texture_array != nullptr;
			if(hastex) {
				// If model->material.texture is nullptr a operator overload will change to the default texture
				// So i dont have to check if is cur_texture is nullptr or not
				if(model->material->texture->get_id() != cur_texture->get_id()) {
					cur_texture->unbind();
					cur_texture = model->material->texture;
					cur_texture->bind(0); // Unit 0
				}
			}

			// Use texture array if it exists
			if(hastexarray) {
				// Bind first time or change texture array
				if(cur_texarray == nullptr || model->material->texture_array->get_id() != cur_texarray->get_id()) {
					// cur_texarray->unbind();
					cur_texarray = model->material->texture_array;
					cur_texarray->bind(1); // Unit 1
				}
				// LOG_DEBUG("Texture index: %d", model->material->texture_array->texture_index);
				cur_shader->set_int("texlayer", model->material->texture_array->texture_index);
			}

			// Not need for cur_mixamount because this is irrelevant
			const float mix_amount = (hastex && hastexarray) ? model->material->mix_amount : (hastexarray ? 1.0f : 0.0f);
			cur_shader->set_float("mixAmount", mix_amount);
			// TEXTURE -- //

			// -- COLOR //
			if(model->material->color != cur_color) {
				cur_shader->set_color("shapeColor", model->material->color);
			}
			// COLOR -- //

			// Draw model
			model->draw_logic(camera, *cur_shader);
		}
	}

	cur_shader->unbind();
	cur_texture->unbind();
	if(cur_texarray != nullptr) {
		cur_texarray->unbind();
	}
	glBindVertexArray(0);
}

