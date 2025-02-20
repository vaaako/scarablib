#pragma once

#include "scarablib/typedef.hpp"

// Object used to represent an image
struct Image {
	int width, height, nr_channels;
	uint8* data = nullptr;
	const char* path;

	// Load an image using a path
	Image(const char* path, const bool flip_vertically = false, const bool flip_horizontally = false) noexcept;
	~Image() noexcept;
};
