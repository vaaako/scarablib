#pragma once

#include "scarablib/gfx/shape2d.hpp"

// Triangle shape object, used to draw triangles
struct Triangle : public Shape2D {
	// Initialize current shape using the Shape2DConf struct
	Triangle(const Shape2DConf& conf);

	// Scene2D call this method.
	// Draw current shape using shader defined by Scene2D class
	void draw(const Shader& shader) override;
};
