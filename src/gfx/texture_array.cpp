#include "scarablib/gfx/texture_array.hpp"
#include "scarablib/gfx/image.hpp"
#include "scarablib/gfx/texturebase.hpp"
#include <algorithm>

TextureArray::TextureArray(const uint16 width, const uint16 height, const uint16 max_textures, const uint8 channels)
	: TextureBase(width, height), max_textures(max_textures), channels(channels) {

	// Set number of textures
	GLint max_layers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
	this->max_textures = std::clamp((uint32)max_textures, 1u, (uint32)max_layers);

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);

	// Allocate data
	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY,
		1, // Mipmap level
		TextureBase::extract_format(channels, true),
		(int)width, (int)height,
		(int)max_textures
	);

	// Set default filters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

uint16 TextureArray::add_texture(const char* path, const bool flip_vertically, const bool flip_horizontally) {
	// Check if limit has been reached
	if(this->num_textures >= this->max_textures) {
		throw ScarabError("Texture array limit (%i) has been reached!", this->num_textures);
	}

	if(path == nullptr) {
		throw ScarabError("Texture has null path");
	}

	Image* image = new Image(path, flip_vertically, flip_horizontally);
	if(image->data == nullptr) {
		throw ScarabError("Image (%s) was not found", image->path);
	}

	// Validate dimensions
	if((image->width != (int)this->width) || (image->height != (int)this->height)) {
		delete image;
		throw ScarabError("Image (%s) has incorrect dimensions. Dimensions must be: %ix%i", path, this->width, this->height);
	}

	// Validate channels
	if(image->nr_channels > (int)this->channels) {
		throw ScarabError("Image (%s) has more channels (%i) than expected (%i)", path, image->nr_channels, this->channels);
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	// Upload to texture array
	glTexSubImage3D(
		GL_TEXTURE_2D_ARRAY,
		0, // Mipmap level
		0, 0, this->num_textures, // x, y, layer (z)
		this->width, this->height,
		1, // Depth
		TextureBase::extract_format(image->nr_channels, false),
		GL_UNSIGNED_BYTE,
		image->data
	);
	delete image;
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	uint16 index = this->num_textures++; // Increment index
	return index;
}

uint16 TextureArray::add_textures(const std::vector<TextureArray::Config>& paths) {
	uint16 index = 0;
	for(size_t i = 0; i < paths.size(); i++) {
		const TextureArray::Config& tex = paths[i];
		index = this->add_texture(tex.path, tex.flip_vertically, tex.flip_horizontally);
	}
	return index;
}

