#pragma once

#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

// Billboard model object
struct Billboard : public Model {
	enum : uint8 {
		FRONT      = 1 << 0,
		FRONTRIGHT = 1 << 1,
		RIGHT      = 1 << 2,
		BACKRIGHT  = 1 << 3,
		BACK       = 1 << 4,
		BACKLEFT   = 1 << 5,
		LEFT       = 1 << 6,
		FRONTLEFT  = 1 << 7
	};

	// Directions the billboard can face
	enum Direction : uint8 {
		NORTH,
		NORTHEAST,
		EAST,
		SOUTHEAST,
		SOUTH,
		SOUTHWEST,
		WEST,
		NORTHWEST
	};

	Billboard() noexcept;
	// This is overrided because if not, it wouldn't be possible to clean the textures
	~Billboard() noexcept override;

	// Sets up directional billboard textures that automatically change based on the camera's angle.
	// Parameters:
	// - `paths`: A list of 4 or 8 texture file paths. These represent the visual appearance from different viewing angles.
	// - `flip`: A bitmask indicating which textures should be mirrored horizontally.
	//           For example, `FRONTRIGHT | RIGHT | BACKRIGHT` or `2 | 4 | 8`.
	//
	// Texture Ordering:
	// - For **4 directions**, textures must be provided in this order:
	//     FRONT, RIGHT, BACK, LEFT
	// - For **8 directions**, textures must be in this order:
	//     FRONT, FRONTRIGHT, RIGHT, BACKRIGHT, BACK, BACKLEFT, LEFT, FRONTLEFT.
	//
	// Flip Behavior:
	// - When a texture is flagged for flipping, a mirrored version is automatically generated.
	//   The flipped texture is then placed at the symmetrical position relative to the FRONT.
	//   For example, if texture at index 2 is marked to flip (LEFT), its mirrored version will be placed at index 6 (RIGHT).
	//
	// Example usage:
	//     set_directional_textures({"front.png", "frontright.png", "right.png", ...}, FRONTRIGHT | RIGHT);
	void set_directional_textures(const std::vector<const char*> paths, const uint32 flip);

	// Updates the billboard's texture direction based on a point position (usually the camera)
	void update_facing_texture(const vec3<float>& point_pos) noexcept;

	// Returns the current direction the billboard is facing
	inline Billboard::Direction get_direction() const noexcept {
		return this->base_dir;
	}

	// Set a new direction to the billboard to face
	inline void set_direction(const Billboard::Direction dir) {
		this->base_dir = dir;
	}

	// Returns the shader used by the billboard
	inline Shader* get_shader() const noexcept override {
		return this->shader;
	};

	private:
		// Precalculated directions
		const float directions[8] = {
			M_PI_2f,        // [ 90°] NORTH       (FROnT)
			M_PI_4f,        // [ 45°] NORTHEAST   (FRONT-RIGHT)
			0.0f,           // [  0°] EAST        (RIGHT)
			7.0f * M_PI_4f, // [315°] SOUTHEAST   (BACK-RIGHT)
			3.0f * M_PI_2f, // [270°] SOUTH       (BACK)
			5.0f * M_PI_4f, // [225°] SOUTHWEST   (BACK-LEFT)
			M_PIf,          // [180°] WEST        (LEFT)
			3.0f * M_PI_4f, // [135°] NORTHWEST   (FRONT-LEFT)
		};
		// Scoped macro
		static constexpr float M_PI2 = M_PIf * 2.0f;

		// Directional textures
		std::vector<std::shared_ptr<Texture>> textures;
		// Precalculated number of textures (either 4 or 8)
		size_t num_sectors = 0;
		// Billboard relative front direction
		Billboard::Direction base_dir = Billboard::Direction::EAST;
		// Used to check if texture must change
		uint32 cur_sector = 9; // Unbounded value to change the first time
		// Pre calculated angle steps between directions
		float angle_step = 0; // Using radians in this case is more accurate

		// Store one time only (deleted inside window destructor)
		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"billboard",
			Shaders::BILLBOARD_VERTEX,
			Shaders::DEFAULT_FRAGMENT
		);

		void draw_logic(const Camera& camera, const Shader& shader) noexcept override;
		void clear_textures() noexcept;
};
