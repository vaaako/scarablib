#pragma once

#include "scarablib/shapes/shape2d.hpp"

// Triangle shape object, used to draw triangles
struct Triangle : protected Shape2D {
	// Initialize current shape using the following values:
	// `position`: Shape's position on the screen
	// `size`: Size of the shape in pixels
	// `color`: Shape's color (white by default)
	// `angle`: Shape's angle (0.0f by default)
	Triangle(const vec2<float>& position, const vec2<float>& size, const Color& color = Colors::WHITE, const float angle = 0.0f);

	// Scene2D call this method.
	// Draw current shape using shader defined by Scene2D class
	void draw(const Shader& shader) override;
};
