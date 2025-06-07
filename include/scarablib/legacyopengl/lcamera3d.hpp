#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/window/window.hpp"

// 3D camera for legacy opengl
class LCamera3D {
	public:
		// Camera's position.
		// To move the camera using a unit vector, use `Camera::move(const vec3<float>& dir, const float dt)`
		vec3<float> position = vec3<float>(0.0f);
		// Camera's Field of View
		float fov;
		// Camera's sensitivity
		float sensitivity;
		// Horizontal rotation.
		// To rotate camera using the mouse, use `Camera::rotate(const MouseHandler& mouse)`
		float yaw = 0.0f;
		// Vertical rotation
		// To rotate camera using the mouse, use `Camera::rotate(const MouseHandler& mouse)`
		float pitch = 0.0f;
		// Camera's near plane
		float znear = 0.1f;
		// Camera's far plane
		float zfar = 1000.0f;

		LCamera3D(const Window& window, const float fov = 45.0f, const float sensitivity = 0.1f) noexcept;

		// Calculates the camera's model view matrix
		void begin() const noexcept;

		inline void end() const noexcept {

		}

	private:
		const Window& window;
};
