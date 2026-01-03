#include "scarablib/gfx/texture.hpp"
#include "scarablib/gfx/image.hpp"
#include "scarablib/gfx/texturebase.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <SDL2/SDL_render.h>

Texture::Texture() noexcept : TextureBase(GL_TEXTURE_2D, 1, 1) {
	// For a default white texture, swizzling can ensure it returns a solid
	// color without uploading actual data
	
	// Swizzle for white texture
	const GLint swizzle_mask[] = { GL_ONE, GL_ONE, GL_ONE, GL_ONE };

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1, GL_RGBA8, 1, 1); // Allocate immutable storage
	glTextureParameteriv(this->id, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
#else
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1, 1); // Allocate without data
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
}

Texture::Texture(const char* path, const bool flip_horizontally, const bool flip_vertically)
	// Placeholder for width and height
	: TextureBase(GL_TEXTURE_2D, 1, 1) {

	Image* image = new Image(path, flip_horizontally, flip_vertically);
	if(image->data == nullptr) {
		delete image;
		throw ScarabError("Image (%s) was not found", image->path);
	}

	this->width  = image->width;
	this->height = image->height;

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1,
		TextureBase::extract_format(image->nr_channels, true),
		image->width,
		image->height
	);
	glTextureSubImage2D(this->id,
		0,
		0, 0,
		image->width, image->height,
		Texture::extract_format(image->nr_channels, false),
		GL_UNSIGNED_BYTE,
		image->data
	);
	glGenerateTextureMipmap(this->id);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);
#else
	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	// Allocate data
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		TextureBase::extract_format(image->nr_channels, true),
		image->width, image->height, 0,
		TextureBase::extract_format(image->nr_channels, false),
		GL_UNSIGNED_BYTE,
		image->data
	);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);

	// Generate mipmap and unbind
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif

	delete image;

#ifdef SCARAB_DEBUG_TEXTURE
	LOG_INFO("Texture loaded succesfully! Width: %d, Height: %d", surface->w, surface->h);
#endif
}

Texture::Texture(const uint8* data, const uint32 width, const uint32 height, const uint8 channels)
	: TextureBase(GL_TEXTURE_2D, width, height) {

	if(data == nullptr) {
		throw ScarabError("Data for texture is null");
	}

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1, TextureBase::extract_format(channels, true), width, height);
	glTextureSubImage2D(this->id, 0, 0, 0, width, height, TextureBase::extract_format(channels, false), GL_UNSIGNED_BYTE, data);

	glGenerateTextureMipmap(this->id);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);
#else
	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	// Allocate data
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		internal_channels,
		width, height, 0,
		channels,
		GL_UNSIGNED_BYTE,
		data
	);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);

	// Generate mipmap and unbind
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif

#ifdef SCARAB_DEBUG_TEXTURE
	LOG_INFO("Texture loaded succesfully! Width: %d, Height: %d", surface->w, surface->h);
#endif
}



Texture::Texture(const Image& image)
	: TextureBase(GL_TEXTURE_2D, image.width, image.height) {

	if(image.data == nullptr) {
		throw ScarabError("Image (%s) was not found", image.path);
	}

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1,
		TextureBase::extract_format(image.nr_channels, true),
		image.width,
		image.height
	);
	glTextureSubImage2D(this->id,
		0,
		0, 0,
		image.width, image.height,
		Texture::extract_format(image.nr_channels, false),
		GL_UNSIGNED_BYTE,
		image.data
	);
	glGenerateTextureMipmap(this->id);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);
#else
	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	// Allocate data
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		TextureBase::extract_format(image->nr_channels, true),
		image->width, image->height, 0,
		TextureBase::extract_format(image->nr_channels, false),
		GL_UNSIGNED_BYTE,
		image->data
	);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);

	// Generate mipmap and unbind
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif

#ifdef SCARAB_DEBUG_TEXTURE
	LOG_INFO("Texture loaded succesfully! Width: %d, Height: %d", surface->w, surface->h);
#endif
}
