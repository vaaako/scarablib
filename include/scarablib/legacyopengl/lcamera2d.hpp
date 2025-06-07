#pragma once

#include "scarablib/window/window.hpp"

// 2D Camera for legacy opengl
class LCamera2D {
	public:
		// Camera's position
		float x;
		// Camera's position
		float y;
		// Camera's zoom
		float zoom;

		LCamera2D(const Window& window, const float x = 0.0f, const float y = 0.0f, const float zoom = 1.0f) noexcept;

		// Calculate the camera's model view matrix.
		// Rotates based on Pitch and Yaw.
		// Moves based on X, Y and Z
		void begin() const noexcept;

		inline void end() const noexcept {

		}

	private:
		const Window& window;
};
