#include "scarablib/types/texture.hpp"
#include "scarablib/typedef.hpp"
#include <SDL2/SDL_render.h>

Texture::Texture() noexcept {
	// Generate and bind texture
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(GL_TEXTURE_2D, this->id);

	// Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	const uint8 data[4] = { 255, 255, 255, 255 }; 

	// Generate
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const Texture::Config& conf) {
	// STB use
	Image* image = new Image(conf.path, conf.flip_vertically, conf.flip_horizontally);
	if(image->data == nullptr) {
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)conf.filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)conf.filter);

	// Repeat, Mirrored Repeat, Clamp to Edge, Clamp to Border (then use array of RGBA to color the border)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)conf.wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)conf.wrap);

	// Generate
	const GLenum format = Texture::extract_format(*image);
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);

	// Generate mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

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
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, static_cast<GLint>(width), static_cast<GLint>(height), 0, format, GL_UNSIGNED_BYTE, data);

	// Gen mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() noexcept {
	// Just in case
	if(this->id != 0) {
		glDeleteTextures(1, &this->id);
	}
}


// void Texture::update_data(const void* data, const GLenum format) {
// 	if(data == nullptr) {
// 		throw ScarabError("Texture data is null");
// 	}
//
// 	glBindTexture(GL_TEXTURE_2D, this->id);
// 	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(this->width), static_cast<GLsizei>(this->height), format, GL_UNSIGNED_BYTE, data);
// 	glGenerateMipmap(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// }

