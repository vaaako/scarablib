#include "scarablib/gfx/texturebase.hpp"
#include "scarablib/proper/error.hpp"

TextureBase::TextureBase(const GLint texturetype, const uint16 width, const uint16 height, const uint32 id) noexcept
	: id(id), width(width), height(height), texturetype(texturetype) {}

TextureBase::~TextureBase() noexcept {
	glDeleteTextures(1, &this->id);
}

void TextureBase::set_filter(const TextureBase::Filter filter) const noexcept {
#if !defined(BUILD_OPGL30)
	glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, (GLint)filter);
	glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, (GLint)filter);
#else
	this->bind();
	// Nearest: Pixelate
	// Linear: Blur
	glTexParameteri(this->texturetype, GL_TEXTURE_MIN_FILTER, (GLint)filter);
	glTexParameteri(this->texturetype, GL_TEXTURE_MAG_FILTER, (GLint)filter);
	this->unbind();
#endif
}

void TextureBase::set_wrap(const TextureBase::Wrap wrap) const noexcept {
#if !defined(BUILD_OPGL30)
	glTextureParameteri(this->id, GL_TEXTURE_WRAP_S, (GLint)wrap);
	glTextureParameteri(this->id, GL_TEXTURE_WRAP_T, (GLint)wrap);
#else
	this->bind();
	// Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border (then use array of RGBA to color the border)
	glTexParameteri(this->texturetype, GL_TEXTURE_WRAP_S, (GLint)wrap);
	glTexParameteri(this->texturetype, GL_TEXTURE_WRAP_T, (GLint)wrap);
	this->unbind();
#endif
}

uint32 TextureBase::extract_format(const uint8 num_channels, const bool internal) {
	switch (num_channels) {
		case 1:
			return (internal) ? GL_R8 : GL_RED;
			break;
		case 3:
			return (internal) ? GL_RGB8 : GL_RGB;
			break;
		case 4:
			return (internal) ? GL_RGBA8 : GL_RGBA;
			break;
		default:
			throw ScarabError("Failed to load texture. Unsupported format of %i channels", num_channels);
	}
}

