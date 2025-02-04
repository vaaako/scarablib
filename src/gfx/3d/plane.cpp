#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/gfx/model.hpp"
#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/proper/log.hpp"

Plane::Plane(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(conf, vertices, indices) {}


// Overrided from Mesh, so is needed to release the vao_mesh
// This is overrided because if not, it wouldn't be possible to clean the textures
Plane::~Plane() noexcept {
	// Release current vao
	VAOManager::get_instance().release_vao(vao_hash);

	for(Texture* texture : this->textures) {
		delete texture;
	}
}

// // Rotate 4 directions
void Plane::rotate_four_directions(const vec3<float>& focus_position) noexcept {
	// Front: -45° <= angle < 45°
	// Right: 45° <= angle < 135°
	// Back: 135° <= angle < 225°
	// Left: 225° <= angle < 315°

	const float angle = this->direction_angle(focus_position);

	// Front
	if(angle >= -45.0f && angle < 45.0f) {
		this->set_texture(this->textures.at(0));
	// Right
	} else if(angle >= 45.0f && angle < 135.0f) {
		this->set_texture(this->textures.at(1));
	// Back
	} else if(angle >= 135.0f && angle < 225.0f) {
		this->set_texture(this->textures.at(2));
	// Left
	} else {
		this->set_texture(this->textures.at(3));
	}
}

// TODO: Instead of set_texture a more optimized method
//	+ Maybe changing texture_id
//	+ Use texture_id on Model instead of Texture object

// Rotate 8 directions
void Plane::rotate_eight_directions(const vec3<float>& focus_position) noexcept {
	const float angle = this->direction_angle(focus_position);

	// 8 Directions: Divide the circle into 8 sectors of 45° each
	// Front Right
	if (angle >= -157.5f && angle < -112.5f) {
		this->set_texture(this->textures.at(1)); 
	// Right
	} else if (angle >= -112.5f && angle < -67.5f) {
		this->set_texture(this->textures.at(2));
	// Back Right
	} else if (angle >= -67.5f && angle < -22.5f) {
		this->set_texture(this->textures.at(3));
	// Back
	} else if (angle >= -22.5f && angle < 22.5f) {
		this->set_texture(this->textures.at(4));
	// Back Left
	} else if (angle >= 22.5f && angle < 67.5f) {
		this->set_texture(this->textures.at(5));
	// Left
	} else if (angle >= 67.5f && angle < 112.5f) {
		this->set_texture(this->textures.at(6));
	// Front Left
	} else if (angle >= 112.5f && angle < 157.5f) {
		this->set_texture(this->textures.at(7));
	// Front
	} else {
		this->set_texture(this->textures.at(0));
	}
}


// TODO: Instead of making a whole new texture for flipped ones, send a uniform for shader flip

void Plane::set_angle_textures(const std::vector<std::pair<const char*, bool>> paths) {
	if(paths.size() < 4 || paths.size() > 8) {
		throw ScarabError("The number of textures must be at least 4 or at most 8");
	}

	// 4 for four directions
	// 5 for flipping textures
	const size_t texture_size = (paths.size() != 4 || paths.size() == 5) ? 8 : 4;

	// Resize so i can check if a index has a texture already
	this->textures.resize(texture_size, nullptr);

	for(size_t i = 0; i < paths.size(); i++) {
		const auto& pair = paths.at(i);

		LOG_DEBUG("Placing at index %zu", i);

		// Place the texture in the eorrect direction
		this->textures[i] = new Texture({
			.path = pair.first,
			.flip_horizontally = false
		});

		// Check for opposite texture
		if(pair.second) {
			// Calculate the opposite index
			const size_t opposite_index = (texture_size - i);

			if(this->textures.at(opposite_index) != nullptr) {
				throw ScarabError("Bad Configuration: Index %zu already has a texture", opposite_index);
			}

			// Place the texture in the correct direction
			this->textures[opposite_index] = new Texture({
				.path = pair.first,
				.flip_horizontally = true
			});

			LOG_DEBUG("+ Placing opposite %zu", opposite_index);
		}
	}

	if(this->textures.size() != 4 && this->textures.size() != 8) {
		throw ScarabError("Bad Configuration: The textures final size is %zu, which does not match 4 or 8. \nCheck for flipped textures", this->textures.size());
	}
}

// TODO: I have to and will optimize this, this is just testing
// Plane::Direction Plane::get_direction(const float angle) const noexcept {
// 	// 8 Directions: Divide the circle into 8 sectors of 45° each
// 	if (angle >= -22.5f && angle < 22.5f) {
// 		return Direction::FRONT;
// 	} else if (angle >= 22.5f && angle < 67.5f) {
// 		return Direction::FRONT_RIGHT;
// 	} else if (angle >= 67.5f && angle < 112.5f) {
// 		return Direction::RIGHT;
// 	} else if (angle >= 112.5f && angle < 157.5f) {
// 		return Direction::BACK_RIGHT;
// 	} else if (angle >= 157.5f && angle < 202.5f) {
// 		return Direction::BACK;
// 	} else if (angle >= 202.5f && angle < 247.5f) {
// 		return Direction::BACK_LEFT;
// 	} else if (angle >= 247.5f && angle < 292.5f) {
// 		return Direction::LEFT;
// 	} else if (angle >= 292.5f && angle < 337.5f) {
// 		return Direction::FRONT_LEFT;
// 	} else {
// 		return Direction::FRONT;  // Handle angle wrapping (e.g., if it's -180° to -157.5°)
// 	}
// }
