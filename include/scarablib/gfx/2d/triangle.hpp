#pragma once

#include "scarablib/gfx/2d/sprite.hpp"

// Class for Triangle sprite
struct Triangle : public Sprite {
	// WARNING: Do not use this constructor, use SpriteFactory
	Triangle(const std::vector<Vertex>& vertices) noexcept;
};

