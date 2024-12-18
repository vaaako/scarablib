#include "scarablib/types/texture.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/image.hpp"
#include <SDL2/SDL_render.h>

Texture::Texture() {
	// Generate and bind texture
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(this->tex_type, this->id);

	// Filter
	glTexParameteri(this->tex_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->tex_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	const uint8 data[4] = { 255, 255, 255, 255 }; 

	// Generate
	glTexImage2D(this->tex_type, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Unbind
	glBindTexture(this->tex_type, 0);
}

Texture::Texture(const char* path, const TextureFilter filter, const TextureWrap wrap) {
	Image* image = new Image(path, true);

#ifdef SCARAB_DEBUG_TEXTURE
	LOG_INFO("Texture loaded succesfully! Width: %d, Height: %d", surface->w, surface->h);
#endif

	// Generate and bind texture
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(this->tex_type, this->id);

	// Set filter parameters
	// Nearest: Pixelate / Linear: Blur
	glTexParameteri(this->tex_type, GL_TEXTURE_MIN_FILTER, (GLint)filter);
	glTexParameteri(this->tex_type, GL_TEXTURE_MAG_FILTER, (GLint)filter);

	// Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border (then use array of RGBA to color the border)
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_S, (GLint)wrap);
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_T, (GLint)wrap);

	// Get format
	GLenum format;
	switch (image->nr_channels) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			LOG_ERROR("Failed to load texture (%s). Unsupported format: %d channels", path, image->nr_channels);
			return;
	}

	// Generate
	glTexImage2D(this->tex_type, 0, static_cast<GLint>(format), image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);

	// Generate mipmap
	glGenerateMipmap(this->tex_type);

	// Unbind
	glBindTexture(this->tex_type, 0);

	delete image;
}
 
Texture::Texture(const void* data, const uint32 width, const uint32 height, const GLint internal_format, const GLenum format) {
	// Generate and bind texture
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(this->tex_type, this->id);

	// Filter
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Generate
	glTexImage2D(this->tex_type, 0, internal_format, static_cast<GLint>(width), static_cast<GLint>(height), 0, format, GL_UNSIGNED_BYTE, data);

	// Gen mipmap
	glGenerateMipmap(this->tex_type);

	// Unbind
	glBindTexture(this->tex_type, 0);
}

Texture::~Texture() {
	// Just in case
	if(this->id != 0) {
		glDeleteTextures(1, &this->id);
	}
}

