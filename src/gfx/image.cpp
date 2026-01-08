#include "scarablib/gfx/image.hpp"

// STB entry point
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image::Image(const char* path, const bool flip_h, const bool flip_v) noexcept : path(path) {
	// Opposite because stbi acts the opposite
	stbi_set_flip_vertically_on_load(!flip_v);

	this->data = stbi_load(path, &this->width, &this->height, &this->channels, 0); // STBI_rgb_alpha to standarlize

	// This gives skybox a nice effect
	// this->data = stbi_load(path, &this->width, &this->height, &channels, STBI_rgb_alpha); // STBI_rgb_alpha to standarlize

	if(this->data == NULL) {
		this->data = nullptr; // For comparasion
		return;
	}

	if(flip_h) {
		// Flip horizontally
		int rowsize = this->width * this->channels * sizeof(uint8);
		for(int y = 0; y < this->height; y++) {
			uint8* row = this->data + (y * rowsize);
			for(int x = 0; x < this->width / 2; x++) {
				for(int c = 0; c < this->channels; c++) {
					std::swap(row[x * this->channels + c], row[(this->width - 1 - x) * this->channels + c]);
				}
			}
		}
	}
}

Image::Image(const uint8* data, const size_t size, const bool flip_h, const bool flip_v) noexcept {
	// Opposite because stbi acts the opposite
	stbi_set_flip_vertically_on_load(!flip_v);

	this->data = stbi_load_from_memory(data, (int)size, &this->width, &this->height, &this->channels, 0);

	if(this->data == NULL) {
		this->data = nullptr; // For comparasion
		return;
	}

	if(flip_h) {
		// Flip horizontally
		int rowsize = this->width * this->channels * sizeof(uint8);
		for(int y = 0; y < this->height; y++) {
			uint8* row = this->data + (y * rowsize);
			for(int x = 0; x < this->width / 2; x++) {
				for(int c = 0; c < this->channels; c++) {
					std::swap(row[x * this->channels + c], row[(this->width - 1 - x) * this->channels + c]);
				}
			}
		}
	}
}

Image::Image(uint8* data, const uint32 width, const uint32 height, const uint8 channels) noexcept
	: path(nullptr), data(data), width((int)width), height((int)height), channels((int)channels) {}

Image::~Image() noexcept {
	if(this->owns_data && this->data != nullptr) {
		stbi_image_free(this->data);
	}
}
