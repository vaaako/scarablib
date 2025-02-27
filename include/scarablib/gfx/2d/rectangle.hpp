#pragma once

#include "scarablib/gfx/2d/sprite.hpp"

// Class for rectangle sprite
struct Rectangle : public Sprite {
	// WARNING: Do not use this constructor, use SpriteFactory
	Rectangle(const std::vector<Vertex>& vertices) noexcept;
};
