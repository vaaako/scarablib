#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/geometry/geometry_factory.hpp"
#include "scarablib/geometry/model.hpp"
#include "scarablib/gfx/image.hpp"
#include <cstddef>
#include <cstdlib>

Billboard::Billboard() noexcept
	: Model(GeometryFactory::make_plane_vertices(), std::vector<uint8> { 0, 1, 2, 0, 2, 3 }) {

	this->material->shader = ShaderManager::get_instance().load_shader_program({
		// Default vertex and fragment shader source
		{ .source = Shaders::BILLBOARD_VERTEX, .type = Shader::Type::Vertex },
		{ .source = Shaders::DEFAULT_FRAGMENT, .type = Shader::Type::Fragment },
	});
}


void Billboard::draw_logic(const Camera& camera) noexcept {
	this->update_model_matrix();

	std::shared_ptr<ShaderProgram> shader = this->material->shader; // cache
	shader->set_matrix4f("proj", camera.get_proj_matrix());
	shader->set_matrix4f("view", camera.get_view_matrix());

	// Billboard stuff
	shader->set_vector3f("billpos", this->position);
	shader->set_float("billsize", this->scale->x);

	// hard coded indices size
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
}

void Billboard::set_directional_textures(const std::vector<const char*> paths, const uint32 flip) {
	if(paths.size() < 4 || paths.size() > 8) {
		throw ScarabError("Minimum of 4 and maximum of 8 images are supported");
	}

	const uint8 final_size = (paths.size() <= 4) ? 4 : 8;

	// Load image to get dimensions and channels
	Image* image = new Image(paths[0], false);
	this->material->texture_array = new TextureArray(image->width, image->height, final_size, image->nr_channels);
	delete image;

	for(size_t i = 0; i < paths.size(); i++) {
		// Add base texture
		this->material->texture_array->add_texture(paths[i], false);

		#ifdef SCARAB_DEBUG_BILLBOARD_TEXTURE
		LOG_DEBUG("Setting texture %zu to %s", i, paths[i]);
		#endif

		// Check if should be flipped
		if(flip & (1 << i)) {
			const size_t opposite_index = (final_size - i) % final_size;

			#ifdef SCARAB_DEBUG_BILLBOARD_TEXTURE
			LOG_DEBUG("Flipping %zu and placing at %zu", i, opposite_index);
			#endif

			// this->textures[opposite_index] = std::make_shared<Texture>(paths[i], true);
			this->material->texture_array->add_texture(paths[i], true, false, opposite_index);
		}
	}

	// Check if matches
	if(this->material->texture_array->get_num_textures() != final_size) {
		throw ScarabError(
			"Texture configuration error: The final texture count must be %u. Check flip configuration.",
			final_size
		);
	}

	this->material->texture_array->generate_mipmap();

	// this->num_sectors = this->textures.size();
	this->angle_step = M_PI2 / (float)this->material->texture_array->get_num_textures();
}

void Billboard::update_facing_texture(const vec3<float>& point_pos) noexcept {
	if(this->material->texture_array == nullptr) {
		return;
	}

	const float dx = point_pos.x - this->position->x;
	const float dz = point_pos.z - this->position->z;

	// If camera is very close, avoid atan2(0, 0)
	if(std::abs(dx) < 0.001f && std::abs(dz) < 0.001f) {
		return;
	}

	// Angle of the camera relative to the world's positive Z-axis
	// This is inverted to get the result that i want
	// Invert back if the intention is the billboard change relative to some external object
	const float angle_to_target = std::atan2(dx, dz);

	// Get the billboard's forward-facing angle in radians
	const float forward_angle = this->directions[this->direction];
	// Calculate the angle of the camera relative to the billboard's front
	// The result is the CCW angle from the billboard's front to the camera vector
	const float relative_angle = std::fmod(angle_to_target - forward_angle + M_PI2, M_PI2);
	// offset by half a sector to center the sprites in their viewing cone
	const float centered_angle = std::fmod(relative_angle + (this->angle_step * 0.5f), M_PI2);
	// Calculate the final sector indeex
	const uint32 sector = static_cast<uint32>(centered_angle / this->angle_step);

	// Only update the current sector if it has changed
	if(sector != this->material->texture_array->texture_index) {
		// this->cur_sector = sector;
		this->material->texture_array->texture_index = sector;
	}

	#ifdef SCARAB_DEBUG_BILLBOARD_ANGLE
	LOG_INFO("Angle: %f, RAngle: %f, Sector: %i", angle_to_target, relative_angle, sector);
	#endif
}

