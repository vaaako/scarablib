#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/gfx/color.hpp"
#include "scarablib/gfx/texture_array.hpp"
#include "scarablib/proper/log.hpp"

IScene::IScene(Camera& camera) noexcept : camera(camera) {}

IScene::~IScene() noexcept {
	this->scene.clear();
	this->vao_groups.clear();
}


void IScene::remove_by_key(const std::string_view& key) {
	if(!this->scene.contains(key)) {
		throw ScarabError("Key '%s' was not found", key.data());
	}

	// Remove from the scene vao map
	this->vao_groups.erase(this->scene.at(key)->vertexarray->get_vaoid());
	// Remove from the scene key map
	this->scene.erase(key);
}

// TODO: Not finished i need to check if its working and change a little bit
void IScene::draw(Mesh& mesh) const noexcept {
	std::shared_ptr<ShaderProgram>& shader = mesh.material->shader;
	shader->use();

	shader->set_int("texSampler", 0);      // Bind texture to unit 0
	shader->set_int("texSamplerArray", 1); // Bind to unit 1
	// NOTE: Since shader probably has texSamplerArray, i need to bind so mix function can work

	mesh.vertexarray->bind_vao();

	const MaterialComponent::TextureHandle& texture = mesh.material->texture;
	const TextureArray* texture_array = mesh.material->texture_array;

	const bool hastex      = mesh.material->texture != nullptr;
	const bool hastexarray = texture_array != nullptr;

	if(hastex) {
		texture->bind(0); // Unit 0
	}

	// Use texture array if it exists
	float mix_amount = 0;
	if(hastexarray) {
		texture_array->bind(1); // Unit 1
		shader->set_int("texlayer", texture_array->texture_index);
		// Mix with texture
		mix_amount = (hastex && hastexarray) ? mesh.material->mix_amount : (hastexarray ? 1.0f : 0.0f);
		// Not need for cur_mixamount because this is irrelevant
		shader->set_float("mixamount", mix_amount);
	}

	shader->set_color("shapeColor", mesh.material->color);
	mesh.draw_logic(this->camera);
}

// glUseProgram      - Most expensive change
// glBindTexture     - Very expensive change
// glBindVertexArray - Less expensive change
// glUniform         - Cheapest change

void IScene::draw_all() const noexcept {
	// Cache shader and camera
	Camera& camera = this->camera;

	// Track currently bound objects to avoid redundant bindings
	GLuint cur_vao                               = 0;
	std::shared_ptr<ShaderProgram> cur_shader    = nullptr;
	MaterialComponent::TextureHandle cur_texture = nullptr; // This is not nullptr, this is converted to a texture inside TextureHandle
	TextureArray* cur_texarray                   = nullptr;
	Color cur_color                              = Colors::WHITE;

	// TODO: Make a new material and store all inside of it?

	// Bind defaults
	cur_texture->bind(); // Bind default texture


	

	for(const auto& [vao, models] : this->vao_groups) {
		// Bind VAO if its a different from the currently bound
		if(vao != cur_vao) {
		#if defined(SCARAB_DEBUG_DRAW_ALL)
			LOG_DEBUG("Changing VAO from %d to: %d", cur_vao, vao);
		#endif
			glBindVertexArray(vao);
			cur_vao = vao;
		}

		Shaders::CameraUniformBuffer cam = {
			.view = camera.get_view_matrix(),
			.proj = camera.get_proj_matrix()
		};
		ResourcesManager::get_instance().get_cameraubo().update(&cam, sizeof(cam));

		// Draw all models with this VAO
		for(const std::shared_ptr<Mesh>& model : models) {
			// -- SHADER //
			// Determine which shader to use
			std::shared_ptr<ShaderProgram> model_shader = model->material->shader;
			if(model_shader != cur_shader) {
				cur_shader = model_shader;
				cur_shader->use();
			#if defined(SCARAB_DEBUG_DRAW_ALL)
				LOG_DEBUG("changin shader %d", cur_shader->get_id());
			#endif

				// Bind Again
				cur_shader->set_int("texSampler", 0);      // Bind texture to unit 0
				cur_shader->set_int("texSamplerArray", 1); // Bind to unit 1
				cur_shader->set_color("shapeColor", cur_color);
				// NOTE: Since shader probably has texSamplerArray, i need to bind so mix function can work
			}
			// SHADER -- //


			// -- TEXTURE //

			// Texture is never nullptr, it can be the default texture sometimes
			// no need to check if has texture
			if(model->material->texture->get_id() != cur_texture->get_id()) {
				cur_texture = model->material->texture;
				cur_texture->bind(0); // Unit 0
			}

			// Use texture array if it exists
			const bool hastexarray = model->material->texture_array != nullptr;
			if(hastexarray) {
				// Bind first time or change texture array
				if(cur_texarray == nullptr || model->material->texture_array->get_id() != cur_texarray->get_id()) {
					cur_texarray = model->material->texture_array;
					cur_texarray->bind(1); // Unit 1
				}
				// This correctly changes billboard texture
				cur_shader->set_int("texlayer", model->material->texture_array->texture_index);
			}
			// Not need for cur_mixamount because this is irrelevant
			// NOTE: yeah, it needs to be outside the if-case above, so it changes when a texarray is nullptr
			const float mix_amount = (model->material->texture != nullptr && hastexarray) ? model->material->mix_amount : (hastexarray ? 1.0f : 0.0f);
			cur_shader->set_float("mixamount", mix_amount);

			// TEXTURE -- //

			// -- COLOR //
			if(model->material->color != cur_color) {
			#if defined(SCARAB_DEBUG_DRAW_ALL)
				LOG_DEBUG("changin color (%d, %d, %d, %d)", cur_color.red, cur_color.green, cur_color.blue, cur_color.alpha);
			#endif
				cur_color = model->material->color;
				cur_shader->set_color("shapeColor", cur_color);
			}
			// COLOR -- //

			// Draw model
			model->draw_logic(camera);
		}
	}

	cur_shader->unbind();
	cur_texture->unbind();
	if(cur_texarray != nullptr) {
		cur_texarray->unbind();
	}
	glBindVertexArray(0);
}

