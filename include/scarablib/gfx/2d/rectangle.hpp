#pragma once

#include "scarablib/gfx/shape2d.hpp"

// Rectangle shape object, used to draw rectangle
struct Rectangle : public Shape2D {
	// Initialize current shape using the Shape2DConf struct
	Rectangle(const Shape2DConf& conf) noexcept;

	// Scene2D call this method.
	// Draw current shape using shader defined by Scene2D class
	void draw(const Shader& shader) noexcept override;
};
