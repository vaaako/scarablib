#include "scarablib/types/texture.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <SDL2/SDL_render.h>

Texture::Texture() noexcept {
	// Generate and bind texture
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(GL_TEXTURE_2D, this->id);
	const uint8 data[4] = { 255, 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const char* path, const bool flip_horizontally, const bool flip_vertically) {
	// STB use
	Image* image = new Image(path, flip_horizontally, flip_vertically);
	if(image->data == nullptr) {
		delete image;
		throw ScarabError("Image (%s) was not found", image->path);
	}

	this->width  = static_cast<uint32>(image->width);
	this->height = static_cast<uint32>(image->height);

	#ifdef SCARAB_DEBUG_TEXTURE
	LOG_INFO("Texture loaded succesfully! Width: %d, Height: %d", surface->w, surface->h);
	#endif

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	// Set filter parameters
	// Nearest: Pixelate / Linear: Blur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border (then use array of RGBA to color the border)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate
	const GLenum format = Texture::extract_format(*image, false);
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		static_cast<GLint>(format),
		image->width, image->height, 0,
		format,
		GL_UNSIGNED_BYTE,
		image->data
	);

	glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmap
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind

	delete image;
}

Texture::Texture(const void* data, const uint32 width, const uint32 height, const GLint internal_format, const GLenum format) 
	: width(width), height(height) {

	if(data == NULL) {
		throw ScarabError("Texture data is null");
	}

	// Generate and bind texture
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(GL_TEXTURE_2D, this->id);

	// Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Generate
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		internal_format,
		static_cast<GLint>(width), static_cast<GLint>(height), 0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() noexcept {
	glDeleteTextures(1, &this->id);
}


void Texture::set_filter(const Texture::Filter filter) const noexcept {
	this->bind();

	// Nearest: Pixelate / Linear: Blur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)filter);

	this->unbind();
}

void Texture::set_wrap(const Texture::Wrap wrap) const noexcept {
	this->bind();

	// Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border (then use array of RGBA to color the border)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrap);

	this->unbind();
}

GLuint Texture::extract_format(const Image& image, const bool internal) {
	switch (image.nr_channels) {
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
			throw ScarabError("Failed to load texture (%s). Unsupported format: %d channels", image.path, image.nr_channels);
	}
}
