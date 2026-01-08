#pragma once

#include "scarablib/gfx/texture.hpp"
#include <memory>

struct SubMesh {
	std::shared_ptr<Texture> texture = nullptr;
	// Indices size
	uint32 indices_count = 0;
	// Start position in the master index buffer
	uint32 base_index    = 0;
};
