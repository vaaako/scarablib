#include "scarablib/types/image.hpp"
#include "scarablib/proper/error.hpp"

// STB entry point
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image::Image(const char* path, const bool flip) {
	stbi_set_flip_vertically_on_load(flip);

	this->data = stbi_load(path, &this->width, &this->height, &nr_channels, 0); // STBI_rgb_alpha to standarlize
	if(data == NULL) {
		throw ScarabError("Image [%s] was not found", path);
	}
}

Image::~Image() {
	if(this->data != nullptr) {
		stbi_image_free(this->data);
	}
}
