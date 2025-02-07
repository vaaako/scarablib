#include "scarablib/types/image.hpp"
#include "scarablib/proper/error.hpp"

// STB entry point
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image::Image(const char* path, const bool flip_vertically, const bool flip_horizontally) : path(path) {
	stbi_set_flip_vertically_on_load(flip_vertically);

	this->data = stbi_load(path, &this->width, &this->height, &nr_channels, 0); // STBI_rgb_alpha to standarlize

	// This gives skybox a nice effect
	// this->data = stbi_load(path, &this->width, &this->height, &nr_channels, STBI_rgb_alpha); // STBI_rgb_alpha to standarlize
	if(data == NULL) {
		throw ScarabError("Image (%s) was not found", path);
	}

	if(flip_horizontally) {
		// Flip horizontally
		int rowSize = this->width * this->nr_channels;
		for(int y = 0; y < this->height; y++) {
			unsigned char* row = this->data + (y * rowSize);
			for(int x = 0; x < this->width / 2; x++) {
				for(int c = 0; c < this->nr_channels; c++) {
					std::swap(row[x * this->nr_channels + c], row[(this->width - 1 - x) * this->nr_channels + c]);
				}
			}
		}
	}
}

Image::~Image() noexcept {
	if(this->data != nullptr) {
		stbi_image_free(this->data);
	}
}
