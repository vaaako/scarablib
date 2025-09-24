#pragma once

#include "scarablib/geometry/model.hpp"

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

	// Billboard relative front direction
	Billboard::Direction direction = Billboard::Direction::EAST;

	Billboard() noexcept;

	// Sets up directional billboard textures that automatically change based on the camera's angle.
	// This uses a texture array, so all textures must have the same size.
	// Size and number of channels will be based on the first texture provided.
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

		// Pre calculated angle steps between directions
		float angle_step = 0; // Using radians in this case is more accurate

		void draw_logic(const Camera& camera) noexcept override;
};
