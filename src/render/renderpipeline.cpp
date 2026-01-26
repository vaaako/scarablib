#include "scarablib/render/renderpipeline.hpp"
#include "scarablib/opengl/assets.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/proper/log.hpp"

#define SCARAB_DEBUG_RENDERER

void RenderPipeline::render(const Scene& scene) noexcept {
	this->begin_frame();

	const Camera& camera = *scene.active_camera;
	// Uniform Buffer for Camera
	Shaders::CameraUniformBuffer cam = {
		.view = camera.get_view_matrix(),
		.proj = camera.get_proj_matrix()
	};
	ResourcesManager::u_camera()->update(&cam);
	// Draw all meshes in scene
	this->drawmeshes(scene.meshes);

	this->end_frame();
}

void RenderPipeline::drawmeshes(const std::vector<std::unique_ptr<Mesh>>& meshes) noexcept {
	static GLuint last_vaoid = 0;

	for(const std::unique_ptr<Mesh>& mesh : meshes) {
		if(mesh->vertexarray->get_vaoid() != last_vaoid) {
			last_vaoid = mesh->vertexarray->get_vaoid();
			glBindVertexArray(last_vaoid);
		}

		mesh->update_model_matrix();
		Shaders::TransformUniformBuffer trans = {
			.model = mesh->get_model_matrix()
		};
		ResourcesManager::u_transform()->update(&trans);

		// Bind shader, texture and color
		this->bind_material(*mesh->material);
		mesh->draw_logic();
	}
}

void RenderPipeline::bind_material(Material& material) noexcept {
	static std::shared_ptr<ShaderProgram> last_shader = ResourcesManager::default_shader();
	static std::shared_ptr<Texture> last_texture      = Assets::default_texture();
	static TextureArray* last_texarray                = nullptr;
	static Color last_color                           = Colors::WHITE;
	static float last_mix_amount                      = 0.0f;
	static int last_texlayer                          = 0;

	// -- SHADER //
	std::shared_ptr<ShaderProgram> mesh_shader = material.shader;
	if(mesh_shader != last_shader) {
		last_shader = mesh_shader;
		last_shader->use();

	#if defined(SCARAB_DEBUG_RENDERER)
		LOG_DEBUG("Changing shader to: %d", last_shader->get_programid());
	#endif

		// Always re-bind texture units when shader changes, as uniforms can be reset
		last_shader->set_int("texSampler", 0);
		if(last_shader->has_uniform("texSamplerArray")) {
			last_shader->set_int("texSamplerArray", 1);
		}
	}
	// SHADER -- //


	// -- TEXTURE //

	bool texchanged = false;
	if(material.texture == nullptr) {
		material.texture = Assets::default_texture();
	}

	if(material.texture->get_id() != last_texture->get_id()) {
		last_texture = material.texture;
		last_texture->bind(0); // Unit 0
		texchanged = true;
	}

	// Use texture array if it exists
	const bool has_texarray = material.texture_array != nullptr;
	bool texarray_changed = false;

	if(has_texarray) {
		if(last_texarray == nullptr || material.texture_array->get_id() != last_texarray->get_id()) {
			last_texarray = material.texture_array;
			last_texarray->bind(1); // Unit 0
			texarray_changed = true;
		}
	} else if(last_texarray != nullptr) {
		// If material no longer has a texture array
		last_texarray = nullptr;
		texarray_changed = true;
	}


	// Determine mix amount and texture layer for the material uniform buffer
	float cur_mix_amount = 0.0f;
	int cur_texlayer = 0;

	if(has_texarray && material.texture->get_id() != Assets::default_texture()->get_id()) {
		cur_mix_amount = material.mix_amount;
		cur_texlayer  = material.texture_array->texture_index;
	} else if(has_texarray) {
		// Only array texture is active
		cur_mix_amount = 1.0f;
		cur_texlayer  = material.texture_array->texture_index;
	} else {
		// Only 2D texture or default texture
		cur_mix_amount = 0.0f;
		cur_texlayer  = 0; // Not relevant
	}
	// TEXTURE -- //

	// Only update Material Uniform Buffer if material properties have actually changed
	if(texchanged || texarray_changed ||
		material.color != last_color ||
		std::abs(cur_mix_amount - last_mix_amount) > 0.001f || // Compra floats with tolerance
		cur_texlayer != last_texlayer ||
		texarray_changed) {

		last_color      = material.color;
		last_mix_amount = cur_mix_amount;
		last_texlayer   = cur_texlayer;

		Shaders::MaterialUniformBuffer mat = {
			.color = last_color.normalize(),
			// x = mixamount, y = texlayer
			.params = { cur_mix_amount, static_cast<float>(cur_texlayer), 0.0f, 0.0f }
		};
		ResourcesManager::u_material()->update(&mat);

		#if defined(SCARAB_DEBUG_RENDERER)
			LOG_DEBUG("Updating Material UBO: Color (%f, %f, %f, %f), MixAmount %f, TexLayer %d",
					mat.color.r, mat.color.g, mat.color.b, mat.color.a,
					mat.params.x, static_cast<int>(mat.params.y));
		#endif
	}
}
