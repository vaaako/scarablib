#pragma once

#include "scarablib/typedef.hpp"

struct SubMesh {
	uint32 textureid = 0;
	// Indices size
	uint32 indices_count = 0;
	// Start position in the master index buffer
	uint32 base_index    = 0;
};
