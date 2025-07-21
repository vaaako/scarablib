#include "scarablib/gfx/texture.hpp"
#include "scarablib/gfx/image.hpp"
#include "scarablib/gfx/texturebase.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <SDL2/SDL_render.h>

Texture::Texture() noexcept : TextureBase(1, 1) {
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	// For a default white texture, swizzling can ensure it returns a solid color without uploading actual data
	GLint swizzle_mask[] = { GL_ONE, GL_ONE, GL_ONE, GL_ONE };
	glTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
	const uint8 data[4] = { 255, 255, 255, 255 };
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 1, 1, 1);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

Texture::Texture(const char* path, const bool flip_horizontally, const bool flip_vertically)
	// Placeholder for width and height
	: TextureBase(1, 1) {

	Image* image = new Image(path, flip_horizontally, flip_vertically);
	if(image->data == nullptr) {
		delete image;
		throw ScarabError("Image (%s) was not found", image->path);
	}

	this->width  = image->width;
	this->height = image->height;

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);

	// Allocate data
	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY,
		1, // Mipmap level
		Texture::extract_format(image->nr_channels, true),
		image->width, image->height,
		1  // num textures
	);

	// Upload texture
	glTexSubImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,       // Mipmap level
		0, 0, 0, // x, y, layer (z)
		image->width, image->height,
		1,       // Depth
		Texture::extract_format(image->nr_channels, false),
		GL_UNSIGNED_BYTE,
		image->data
	);

	delete image;

	// Set filter parameters
	// Nearest: Pixelate
	// Linear: Blur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border (then use array of RGBA to color the border)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate mipmap and unbind
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);

	#ifdef SCARAB_DEBUG_TEXTURE
	LOG_INFO("Texture loaded succesfully! Width: %d, Height: %d", surface->w, surface->h);
	#endif
}

Texture::Texture(const uint8* data, const uint32 width, const uint32 height, const uint32 channels)
	: TextureBase(width, height) {

	if(data == nullptr) {
		throw ScarabError("Data for texture is null");
	}

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);

	// Allocate data
	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY,
		1, // Mipmap level
		TextureBase::extract_format(channels, true),
		width, height,
		1  // num textures
	);

	// Upload texture
	glTexSubImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,       // Mipmap level
		0, 0, 0, // x, y, layer (z)
		width, height,
		1,       // Depth
		TextureBase::extract_format(channels, false),
		GL_UNSIGNED_BYTE,
		data
	);

	// Set filter parameters
	// Nearest: Pixelate
	// Linear: Blur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border (then use array of RGBA to color the border)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate mipmap and unbind
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);

	#ifdef SCARAB_DEBUG_TEXTURE
	LOG_INFO("Texture loaded succesfully! Width: %d, Height: %d", surface->w, surface->h);
	#endif
}

