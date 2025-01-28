#pragma once

#include "scarablib/typedef.hpp"

// Object used to represent an image
struct Image {
	int width, height, nr_channels;
	uint8* data = nullptr;

	// Load an image using a path
	Image(const char* path, const bool flip = true);
	~Image() noexcept;
};
