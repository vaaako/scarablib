#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/window/window.hpp"

// Possible shapes in legacy opengl
namespace LShapes {
	// Draws a rectangle
	void draw_rectangle(const vec2<float>& position, const vec2<float>& size) noexcept;
	// Draws a Cube
	void draw_cube(const vec3<float>& position, const float size, const float angle) noexcept;

	inline void set_color(const Color& color) noexcept {
		glColor4f(color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, color.alpha / 255.0f);
	}
};
