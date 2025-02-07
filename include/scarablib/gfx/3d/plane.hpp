#pragma once

#include "scarablib/gfx/model.hpp"
#include "scarablib/proper/error.hpp"
#include <utility>
#include <vector>

// This is a class to make a plane model.
// WARNING: Do not use this class directly, use ModelFactory::create_cube(const ModelConf& conf)
struct Plane : public Model {
	// Camera's direction relative to the plane.
	enum class Direction : uint8 {
		FRONT,
		FRONT_RIGHT,
		RIGHT,
		BACK_RIGHT,
		BACK,
		BACK_LEFT,
		LEFT,
		FRONT_LEFT
	};
	//          FRONT
	//            ↑
	// FRONT_LEFT   FRONT_RIGHT
	//      ↖           ↗
	// LEFT   ← PLANE →   RIGHT
	//      ↙           ↘
	// BACK_LEFT    BACK_RIGHT
	//            ↓
	//           BACK

	// WARNING: Do not use this class directly, use ModelFactory::create_cube(const ModelConf& conf)
	Plane(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
	// This is overrided because if not, it wouldn't be possible to clean the textures
	~Plane() noexcept override;


	// This will add multiple textures that changes depending on the direction of the plane.
	// Uses a vector of pairs where the first element is the path of the texture.
	// and the second element is a boolean that indicates if the texture should be flipped horizontally.
	// If the texture is flipped, it will be used in the opposite direction (If 4 textures are set, and the LEFT is set to be flipped, the RIGHT will be the LEFT flipped).
	// The vector MUST have minimum of 4 elements and maximum of 6 elements
	// WARNING: Please use this method just at the creation of the plane, if you want to change a texture, use the set_texture_at method
	void set_directional_textures(const std::vector<std::pair<const char*, bool>> paths);
	// You have the option to 4, 8 or 6 textures.
	// 4 textures are equivalent to FRONT, RIGHT, BACK, LEFT, respectively.
	// 8 textures are equivalent to FRONT, FRONT_RIGHT, RIGHT, BACK_RIGHT, BACK, BACK_LEFT, LEFT, FRONT_LEFT respectively
	// 5 texture are equivalent to the same as 8 textures but FRONT_RIGHT, RIGHT and BACK_LEFT are flipped to make FRONT_LEFT, LEFT and BACK_LEFT

	// Run this is you pretend to remove all directional textures
	void clear_directional_textures() noexcept;

	// Changes the texture based on the angle of the plane front face relative to the focus position.
	// Just use this method if you have at least 4 textures set.
	// *WARNING:* This method DOES NOT check the number of textures, you will have a crash if you don't have at least 4 textures
	void rotate_four_directions(const vec3<float>& focus_position) noexcept;

	// Changes the texture based on the angle of the plane front face relative to the focus position.
	// Just use this method if you have 8 textures set.
	// *WARNING:* This method DOES NOT check the number of textures, you will have a crash if you don't have 8 textures
	void rotate_eight_directions(const vec3<float>& focus_position) noexcept;


	// Rotates the plane front face to the camera.
	// position: Camera's position
	// axis: Axis to apply rotation (Y default)
	inline void face_rotation(const vec3<float>& focus_position, const vec3<bool>& axis = { false, true, false }) noexcept {
		// The default angle range of -180 to 180 causes issues because, in OpenGL, -Z is the forward direction.
		// When the plane is oriented at -180 degrees, its front face turns away from the camera,
		// making the back face, face the camera, which gets culled by default.
		//
		// That's why the indices of the plane is flipped in ModelFactory
		this->set_orientation(this->direction_angle(focus_position), axis);
	}

	// Returns the angle of the focus position relative to the plane.
	// In simple terms: Where the camera is relative to the plane? 0.0 is in the FRONT and 180.0 is in the BACK
	inline float direction_angle(const vec3<float>& focus_position) const noexcept  {
		// Focus - Object
		const vec3<float> direction = glm::normalize(focus_position - this->conf.position);

		// Angle between
		return glm::degrees(atan2(direction.x, direction.z));
	}

	// Returns the direction of the plane front face
	Plane::Direction get_direction(const float angle) const noexcept;

	private:
		std::vector<Texture*> textures;
};
