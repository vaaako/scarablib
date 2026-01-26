#include "scarablib/gfx/texture.hpp"
#include "scarablib/gfx/image.hpp"
#include "scarablib/gfx/texturebase.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <SDL2/SDL_render.h>

Texture::Texture() noexcept : TextureBase(GL_TEXTURE_2D, 1, 1) {
	constexpr uint8 white_pixel[4] = { 255, 255, 255, 255 };

#if !defined(BUILD_OPGL30)
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	// Allocate immutable storage
	glTextureStorage2D(this->id,
		1,
		GL_RGBA8,
		1, 1
	);

	// Upload data
	glTextureSubImage2D(this->id,
		0,
		0, 0,
		1, 1,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		white_pixel
	);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);

#else
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA8,
		1, 1, 0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		white_pixel
	);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
#endif

/*
	// Swizzle so the single texel always evaluates to white
	const GLint swizzle_mask[] = { GL_ONE, GL_ONE, GL_ONE, GL_ONE };

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1, GL_RGBA8, 1, 1);
	glTextureParameteriv(this->id, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);

	glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1, 1);
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);

	glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
*/
}

Texture::Texture(const Image& image)
	: TextureBase(GL_TEXTURE_2D, image.width, image.height) {

	if(image.data == nullptr) {
		throw ScarabError("Image (%s) was not found", image.path);
	}

#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1,
		TextureBase::extract_format(image.channels, true),
		image.width,
		image.height
	);
	glTextureSubImage2D(this->id,
		0,
		0, 0,
		image.width, image.height,
		Texture::extract_format(image.channels, false),
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
		TextureBase::extract_format(image->channels, true),
		image->width, image->height, 0,
		TextureBase::extract_format(image->channels, false),
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


Texture::Texture(const uint8* data, const uint32 width, const uint32 height, const uint8 channels)
	: TextureBase(GL_TEXTURE_2D, width, height) {

	if(data == nullptr) {
		throw ScarabError("Texture raw data is null");
	}

#if !defined(BUILD_OPGL30)
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	// Allocate immutable storage
	glTextureStorage2D(this->id,
		1,
		TextureBase::extract_format(channels, true),
		width, height
	);

	// Upload data
	glTextureSubImage2D(this->id,
		0,
		0, 0,
		width, height,
		TextureBase::extract_format(channels, false),
		GL_UNSIGNED_BYTE,
		data
	);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);

#else
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexImage2D(GL_TEXTURE_2D, 0,
		TextureBase::extract_format(channels, true),
		width, height, 0,
		TextureBase::extract_format(channels, false),
		GL_UNSIGNED_BYTE,
		data
	);

	this->set_wrap(TextureBase::Wrap::REPEAT);
	this->set_filter(TextureBase::Filter::NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
#endif
}
