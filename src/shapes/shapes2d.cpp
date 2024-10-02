#include "scarablib/shapes/shape2d.hpp"

Shape2D::Shape2D(const vec2<float>& position, const vec2<float>& size, const Color& color, const float angle)
	: position(position), size(size), color(color), angle(angle) {}

