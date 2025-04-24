#pragma once

#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

// Class for billboard model
struct Billboard : public Model {
	// Directions the billboard can face
	enum Direction : uint8 {
		EAST,
		NORTHEAST,
		NORTH,
		NORTHWEST,
		WEST,
		SOUTHWEST,
		SOUTH,
		SOUTHEAST
	};

	// Supported sectors
	enum : uint8 {
		FOUR_SECTORS = 4,
		EIGHT_SECTORS = 8
	};

	Billboard() noexcept;
	// This is overrided because if not, it wouldn't be possible to clean the textures
	~Billboard() noexcept override;

	// This will add multiple textures that changes depending on the angle of the camera relative to the billboard.
	// - `paths` is a vector of paths to the textures
	// - `flip` is the textures to be flipped (Starting index is 1. Pass 0 to flip none).
	// If the texture is set to flip, the flipped version will be added in the opposite order
	// Example: `123` will flip the respective textures to `567`.
	// The vector MUST have a minimum of 4 elements and a maximum of 8 elements
	// WARNING: Please use this method just at the creation of the billboard
	void config_directional_textures(const std::vector<const char*> paths, uint32 flip);

	// This changes the texture based on the angle of the camera.
	// - `camera_pos` is the position of the camera.
	void update_facing_texture(const vec3<float>& camera_pos) noexcept;
	// First arg isnt camera object to give some freedom

	// Returns the current direction the billboard is facing
	inline Billboard::Direction get_direction() const noexcept {
		return this->cur_dir;
	}

	// Set a new direction to the billboard face.
	inline void set_direction(const Billboard::Direction dir) {
		this->cur_dir = dir;
	}

	// Replace a diretional texture
	inline void set_dir_texture(const uint32 index) noexcept {
		if(index > this->textures.size()) {
			LOG_ERROR_FN("Index out of range");
			return;
		}
		this->set_texture(this->textures[index]);
	}

	// Returns the billboard shader
	inline Shader* get_shader() const noexcept override {
		return this->shader;
	};

	private:
		// Precalculated directions
		const float directions[8] = {
			0.0f,                // [  0°] EAST        (RIGHT)
			M_PI_4f,             // [ 45°] NORTHEAST   (UP-RIGHT)
			M_PI_2f,             // [ 90°] NORTH       (UP)
			2.356194490192345f,  // [135°] NORTHWEST   (UP-LEFT)     3.0f * M_PI_4f
			M_PIf,               // [180°] WEST        (LEFT)
			3.9269908169872414f, // [225°] SOUTHWEST   (DOWN-LEFT)   5.0f * M_PI_4f
			4.71238898038469f,   // [270°] SOUTH       (DOWN)        3.0f * M_PI_2f
			5.497787143782138f   // [315°] SOUTHEAST   (DOWN-RIGHT)  7.0f * M_PI_4f
		};
		static constexpr float M_PI2 = M_PIf * 2.0f;
		std::vector<Texture*> textures; // Directional textures
		size_t num_sectors = 0; // Either 4 or 8 (is the number of directional textures)
		Billboard::Direction cur_dir = Billboard::Direction::EAST; // Only used inside `relative_angle`
		uint32 cur_sector = 9; // Unbounded value to change the first time

		// Store one time only (deleted inside window destructor)
		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"billboard",
			Shaders::BILLBOARD_VERTEX,
			Shaders::DEFAULT_FRAGMENT
		);

		void draw(const Camera& camera, const Shader& shader) noexcept override;
};
