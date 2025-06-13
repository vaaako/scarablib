#pragma once

#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

// Billboard model object
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

	// Configures billboard textures to change with camera angle.
	// - `paths`: provides 4-8 texture paths.
	// - `flip`: specifies textures to mirror.
	// Flipped textures are added in reverse order.
	// Call only during billboard creation.
	// Textures are selected based on camera direction.
	void config_directional_textures(const std::vector<const char*> paths, uint32 flip);

	// Changes the billboard's texture based on the point's position.
	// - `point_pos`: 3D position of the point to look at
	void update_facing_texture(const vec3<float>& point_pos) noexcept;
	// First arg isnt camera object to give some freedom

	// Returns the current direction the billboard is facing
	inline Billboard::Direction get_direction() const noexcept {
		return this->cur_dir;
	}

	// Set a new direction for the billboard to face.
	// - `dir`: The new direction for the billboard
	inline void set_direction(const Billboard::Direction dir) {
		this->cur_dir = dir;
	}

	// Replaces the billboard's texture at a specific directional index.
	// - `index`: The index of the texture to replace
	inline void set_dir_texture(const uint32 index) noexcept {
		if(index > this->textures.size()) {
			LOG_ERROR_FN("Index out of range");
			return;
		}
		this->material.set_texture(this->textures[index]);
	}

	// Returns the sahder used by the billboard
	inline Shader* get_shader() const noexcept override {
		return this->shader;
	};

	private:
		// Precalculated directions
		const float directions[8] = {
			0.0f,           // [  0°] EAST        (RIGHT)
			M_PI_4f,        // [ 45°] NORTHEAST   (UP-RIGHT)
			M_PI_2f,        // [ 90°] NORTH       (UP)
			3.0f * M_PI_4f, // [135°] NORTHWEST   (UP-LEFT)
			M_PIf,          // [180°] WEST        (LEFT)
			5.0f * M_PI_4f, // [225°] SOUTHWEST   (DOWN-LEFT)
			3.0f * M_PI_2f, // [270°] SOUTH       (DOWN)
			7.0f * M_PI_4f  // [315°] SOUTHEAST   (DOWN-RIGHT)
		};
		// Scoped macro
		static constexpr float M_PI2 = M_PIf * 2.0f;

		// Directional textures
		std::vector<Texture*> textures;
		// Precalculated number of textures (either 4 or 8)
		size_t num_sectors = 0;
		// Only used inside `relative_angle`
		Billboard::Direction cur_dir = Billboard::Direction::EAST;
		// Unbounded value to change the first time
		uint32 cur_sector = 9;

		// Store one time only (deleted inside window destructor)
		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"billboard",
			Shaders::BILLBOARD_VERTEX,
			Shaders::DEFAULT_FRAGMENT
		);

		void draw(const Camera& camera, const Shader& shader) noexcept override;
};
