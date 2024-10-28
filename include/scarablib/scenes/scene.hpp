#pragma once

#include "scarablib/window/window.hpp"

// Different modes of drawing shapes
// `OUTLINEMODE` draws the outline of the shape
// `FILLMODE` draws the shape as normal
enum DrawMode : uint32 {
	OUTLINEMODE = GL_LINE,
	FILLMODE = GL_FILL
};

// Virtual class used to make Scene2D and Scene3D
class Scene {
	public:
		// Build scene object using the window object for viewport
		Scene(const Window& window);
		virtual ~Scene() = default;

		// Update viewport using window object
		virtual inline void update_viewport(const Window& window) = 0;

		// Update viewport using width and height values
		virtual void update_viewport(const uint32 width, const uint32 height) = 0;

	protected:
		// Viewport
		uint32 width;
		uint32 height;
};

