#pragma once

#include "scarablib/typedef.hpp"

// Object used to represent an image
struct Image {
	// Image path (may be nullptr)
	const char* path;
	// Bytes
	uint8* data = nullptr;
	int width;
	int height;
	// How many channels (min: 1 and max: 4)
	int channels;
	// Disables free inside this struct
	bool owns_data = true;

	// Load an image using a path
	Image(const char* path, const bool flip_h = false, const bool flip_v = false) noexcept;
	// Load an image from raw data
	Image(const uint8* data, const size_t size, const bool flip_h = false, const bool flip_v = false) noexcept;
	// Set all members manually.
	// `path` is set to `nullptr`
	Image(uint8* data, const uint32 width, const uint32 height, const uint8 channels) noexcept;
	~Image() noexcept;

	// Returns the byte size of the image
	inline constexpr size_t byte_size() const noexcept {
		return size_t(this->width) * size_t(this->height) * size_t(this->channels);
	}
};
