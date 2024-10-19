#include "scarablib/types/texture.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/typedef.hpp"
#include <SDL2/SDL_render.h>

#define STB_IMAGE_IMPLEMENTATION // Enable STB
#include <stb/stb_image.h>


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
	stbi_set_flip_vertically_on_load(true);

	int width, height, nr_channels;
	uint8* data = stbi_load(path, &width, &height, &nr_channels, 0); // STBI_rgb_alpha to standarlize
	if(data == NULL) {
		LOG_ERROR("Failed to load texture \"%s\"", path);
		return;
	}

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
	switch (nr_channels) {
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
			stbi_image_free(data);
			LOG_ERROR("Failed to load texture (%s). Unsupported format: %d channels", path, nr_channels);
			return;
	}

	// Generate
	glTexImage2D(this->tex_type,  0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);

	// Generate mipmap
	glGenerateMipmap(this->tex_type);

	// Free image
	stbi_image_free(data);

	// Unbind
	glBindTexture(this->tex_type, 0);
}
 
Texture::Texture(const uint8* data, const uint32 width, const uint32 height, const GLenum format) {
	// Generate and bind texture
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(this->tex_type, this->id);

	// Filter
	glTexParameteri(this->tex_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->tex_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(this->tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate
	glTexImage2D(this->tex_type, 0, static_cast<GLint>(format), static_cast<GLint>(width), static_cast<GLint>(height), 0, format, GL_UNSIGNED_BYTE, data);

	// Unbind
	glBindTexture(this->tex_type, 0);
}

Texture::~Texture() {
	// Just in case
	if(this->id != 0) {
		glDeleteTextures(1, &this->id);
	}
}

