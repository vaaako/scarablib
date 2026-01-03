#include "scarablib/gfx/texture_array.hpp"
#include "scarablib/gfx/image.hpp"
#include "scarablib/gfx/texturebase.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <algorithm>

TextureArray::TextureArray(const uint16 width, const uint16 height, const uint16 max_layers, const uint8 channels)
	: TextureBase(GL_TEXTURE_2D_ARRAY, width, height), max_layers(max_layers), channels(channels) {

	// Set number of textures
	GLint maxlayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxlayers);
	this->max_layers = std::clamp((uint32)maxlayers, 1u, (uint32)max_layers);

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &this->id);
	glTextureStorage3D(this->id,
		1,
		TextureBase::extract_format(channels, true),
		width, height, this->max_layers
	);

	this->set_filter(TextureBase::Filter::NEAREST);
	this->set_wrap(TextureBase::Wrap::REPEAT);
#else
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

	this->set_filter(TextureBase::Filter::NEAREST);
	this->set_wrap(TextureBase::Wrap::REPEAT);
#endif
}


TextureArray::TextureArray(const std::vector<Image>& images)
	// Placeholder
	: TextureBase(GL_TEXTURE_2D_ARRAY, 1, 1) {

	if(images.empty()) {
		throw ScarabError("Images vector is empty!");
	}

	// Set number of textures
	GLint maxlayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxlayers);
	this->max_layers = std::clamp((uint32)maxlayers, 1u, (uint32)max_layers);

	// Set configuration
	this->width    = images[0].width;
	this->height   = images[0].height;
	this->channels = images[0].channels;

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &this->id);
	glTextureStorage3D(this->id,
		1,
		TextureBase::extract_format(channels, true),
		width, height, this->max_layers
	);

	this->set_filter(TextureBase::Filter::NEAREST);
	this->set_wrap(TextureBase::Wrap::REPEAT);
#else
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

	this->set_filter(TextureBase::Filter::NEAREST);
	this->set_wrap(TextureBase::Wrap::REPEAT);
#endif


	for(size_t i = 0; i < images.size(); i++) {
		const Image& image = images.at(i);

		// Check if limit has been reached
		if(this->num_layers >= this->max_layers) {
			throw ScarabError("Texture array limit (%u) reached", this->max_layers);
		}

		if(image.data == nullptr) {
			throw ScarabError("Image (%s) was not found", image.path);
		}

		// Validate dimensions
		if((image.width != (int)this->width) || (image.height != (int)this->height)) {
			throw ScarabError("Image (%s) dimensions (%ix%i) mismatch (%ix%i)i", image.path, this->width, this->height);
		}

		// Validate channels
		if(image.channels > (int)this->channels) {
			throw ScarabError("(%s) Too many channels in image (%i > %i)", image.path, image.channels, this->channels);
		}

#if !defined(BUILD_OPGL30)
		glTextureSubImage3D(this->id,
			0, // Mipmap level
			0, 0, this->next_layer, // x, y, layer (z)
			this->width, this->height,
			1, // Depth
			TextureBase::extract_format(image.channels, false),
			GL_UNSIGNED_BYTE,
			image.data
		);
#else
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
		// Upload to texture array
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY,
			0, // Mipmap level
			0, 0, this->next_texture, // x, y, layer (z)
			this->width, this->height, 1, // Width, Height, Depth
			TextureBase::extract_format(image.channels, false),
			GL_UNSIGNED_BYTE,
			image.data
		);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
#endif
		this->next_layer++;
	}
}

uint16 TextureArray::add_texture(const char* path, const bool flip_v, const bool flip_h, const int layer) {
	// Check if limit has been reached
	if(this->num_layers >= this->max_layers) {
		throw ScarabError("Texture array limit (%u) reached", this->max_layers);
	}

	if(layer >= (int)this->max_layers) {
		throw ScarabError("Layer (%i) exceeds limit (%u)", layer, this->max_layers);
	}

	if(path == nullptr) {
		throw ScarabError("Texture has null path");
	}

	Image* image = new Image(path, flip_v, flip_h);
	if(image->data == nullptr) {
		throw ScarabError("Image (%s) was not found", image->path);
	}

	// Validate dimensions
	if((image->width != (int)this->width) || (image->height != (int)this->height)) {
		delete image;
		throw ScarabError("Image (%s) dimensions (%ix%i) mismatch (%ix%i)i", path, this->width, this->height);
	}

	// Validate channels
	if(image->channels > (int)this->channels) {
		throw ScarabError("(%s) Too many channels in image (%i > %i)", path, image->channels, this->channels);
	}

#if !defined(BUILD_OPGL30)
	glTextureSubImage3D(this->id,
		0, // Mipmap level
		// Put in specific layer or not (-1)
		0, 0, (layer < 0) ? this->next_layer : layer, // x, y, layer (z)
		this->width, this->height,
		1, // Depth
		TextureBase::extract_format(image->channels, false),
		GL_UNSIGNED_BYTE,
		image->data
	);
#else
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	// Upload to texture array
	glTexSubImage3D(
		GL_TEXTURE_2D_ARRAY,
		0, // Mipmap level
		// Put in specific layer or not (-1)
		0, 0, (layer < 0) ? this->next_texture : layer, // x, y, layer (z)
		this->width, this->height, 1, // Width, Height, Depth
		TextureBase::extract_format(image->channels, false),
		GL_UNSIGNED_BYTE,
		image->data
	);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
#endif

	delete image;

	// Only increment next texture if adding to the end
	if(layer < 0) {
		this->next_layer++;
	}

	uint16 index = this->num_layers++; // Increment index
	return index;
}

uint16 TextureArray::add_textures(const std::vector<TextureArray::Layer>& paths) {
	uint16 index = 0;
	for(size_t i = 0; i < paths.size(); i++) {
		const TextureArray::Layer& tex = paths[i];
		index = this->add_texture(tex.path, tex.flip_v, tex.flip_h);
	}
	this->generate_mipmap();
	return index;
}

