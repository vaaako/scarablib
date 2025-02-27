#include "scarablib/gfx/2d/rectangle.hpp"
#include <cstdio>

Rectangle::Rectangle(const std::vector<Vertex>& vertices) noexcept
	: Sprite(vertices) {}

