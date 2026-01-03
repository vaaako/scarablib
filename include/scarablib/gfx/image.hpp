#pragma once

#include "scarablib/typedef.hpp"

// Object used to represent an image
struct Image {
	const char* path;
	uint8* data = nullptr;
	int width;
	int height;
	int nr_channels;

	// Load an image using a path
	Image(const char* path, const bool flip_horizontally = false, const bool flip_vertically = false) noexcept;
	// Load an image from raw data
	Image(uint8* data, uint32 width, uint32 height, uint8 channels) noexcept;
	~Image() noexcept;
};
