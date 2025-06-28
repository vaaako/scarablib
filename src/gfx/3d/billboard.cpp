#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/geometry/geometry_factory.hpp"
#include "scarablib/geometry/model.hpp"
#include <cstddef>
#include <cstdlib>

Billboard::Billboard() noexcept
	: Model(GeometryFactory::make_plane_vertices(), std::vector<uint8> { 0, 1, 2, 0, 2, 3 }) {}

// Overrided from Mesh, so is needed to release the vao_mesh
// This is overrided because if not, it wouldn't be possible to clean the textures
Billboard::~Billboard() noexcept {
	// Release physics
	// Mesh does this, but this is a overrided destructor
	delete this->physics;

	// Clear directional textures if any
	this->clear_textures();
}


void Billboard::draw_logic(const Camera& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	shader.set_matrix4f("proj", camera.get_proj_matrix());
	shader.set_matrix4f("view", camera.get_view_matrix());
	shader.set_color("shapeColor", this->material.color);

	// Billboard stuff
	shader.set_vector3f("billboardPos", this->position);
	shader.set_float("billboardSize", this->scale->x);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_BYTE, (void*)0);
}


void Billboard::set_directional_textures(const std::vector<const char*> paths, const uint32 flip) {
	if(paths.size() < 4 || paths.size() > 8) {
		throw ScarabError("Minimum of 4 and maximum of 8 images are supported");
	}

	const uint8 final_size = (paths.size() <= 4) ? 4 : 8;

	this->clear_textures(); // This method can be called multiple times

	// Resize and init values with nullptr so i can check if a index has a texture already
	this->textures.resize(final_size, nullptr);

	for(size_t i = 0; i < paths.size(); i++) {
		// Add base texture
		this->textures[i] = std::make_shared<Texture>(paths[i], false);

		// Check if should be flipped
		if(flip & (1 << i)) {
			const size_t opposite_index = final_size - i;

			#ifdef SCARAB_DEBUG_BILLBOARD_TEXTURE
			LOG_DEBUG("Flipping %zu and placing at %zu", i, opposite_index);
			#endif

			this->textures[opposite_index] = std::make_shared<Texture>(paths[i], true);
		}
	}

	// Check if its all correct
	for(size_t i = 0; i < this->textures.size(); i++) {
		if(this->textures[i] == nullptr) {
			throw ScarabError(
				"Texture configuration error: Missing texture at index %zu. "
				"The final texture count must be %u. Check flip configuration.",
				i, final_size
			);
		}
	}

	this->num_sectors = this->textures.size();
	this->angle_step = M_PI2 / (float)this->num_sectors;
}

void Billboard::update_facing_texture(const vec3<float>& point_pos) noexcept {
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
	const float forward_angle = this->directions[this->base_dir];
	// Calculate the angle of the camera relative to the billboard's front
	// The result is the CCW angle from the billboard's front to the camera vector
	const float relative_angle = std::fmod(angle_to_target - forward_angle + M_PI2, M_PI2);
	// offset by half a sector to center the sprites in their viewing cone
	const float centered_angle = std::fmod(relative_angle + (this->angle_step * 0.5f), M_PI2);
	// Calculate the final sector indeex
	const uint32 sector = static_cast<uint32>(centered_angle / this->angle_step);

	// Only update the current sector if it has changed
	if(sector != this->cur_sector) {
		this->cur_sector = sector;
		this->material.texture = this->textures[sector];
	}

	#ifdef SCARAB_DEBUG_BILLBOARD_ANGLE
	LOG_INFO("Angle: %f, RAngle: %f, Sector: %i", angle_to_target, relative_angle, sector);
	#endif
}



void Billboard::clear_textures() noexcept {
	this->textures.clear();
	this->textures.shrink_to_fit(); // Actually releases the allocated memory
}
