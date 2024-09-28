#include "scarablib/types/texture.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <SDL2/SDL_render.h>

#define STB_IMAGE_IMPLEMENTATION // Enable STB
#include <stb/stb_image.h>

Texture::Texture(const char* path, const TextureFilter filter, const TextureWrap wrap) {
	stbi_set_flip_vertically_on_load(true);

	int width, height, nr_channels;
	uint8* data = stbi_load(path, &width, &height, &nr_channels, 0);
	if (data == NULL) {
		throw ScarabError("Failed to load texture \"%s\"", path);
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

	GLenum format;
	if (nr_channels == 1) {
		format = GL_RED;
	} else if (nr_channels == 3) {
		format = GL_RGB;
	} else if (nr_channels == 4) {
		format = GL_RGBA;
	} else {
		throw ScarabError("Failed to load texture (%s). Unsupported format. Supported formats are: JPG, PNG, TGA, BMP, PSD, GIF, HDR and PIC");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps

	// Free image
	stbi_image_free(data);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::Texture(const SDL_Surface* surface, const TextureFilter filter) {
	glGenTextures(1, &this->id); // num of textures, pointer
	glBindTexture(this->tex_type, this->id);

	// Set filter parameters
	// Nearest: Pixelate / Linear: Blur
	glTexParameteri(this->tex_type, GL_TEXTURE_MIN_FILTER, (int)filter);
	glTexParameteri(this->tex_type, GL_TEXTURE_MAG_FILTER, (int)filter);

	// Check before if is JPG or PNG
	GLenum texture_format = 0;
	Uint8 colors = surface->format->BytesPerPixel;

	// 4 Colors
	if(surface->format->BytesPerPixel == 4) {
		if (surface->format->Rmask == 0x000000ff) {
			texture_format = GL_RGBA;
		} else {
			texture_format = GL_BGRA;
		}
	} else {
		if(surface->format->Rmask == 0x000000ff) {
			texture_format = GL_RGB;
		} else {
			texture_format = GL_BGR;
		}
	}

	// TODO -- Add supported formats here? I don't know if it is really necessary

	glTexImage2D(GL_TEXTURE_2D, 0, colors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);

	// Generate Mipmap
	glGenerateMipmap(this->tex_type);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind
}


Texture::~Texture() {
	glDeleteTextures(1, &this->id);
}


// Assigns image to texture object
// the type may beGL_TEXTURE_2D_ARRAY 
// glTexImage3D(this->tex_type, 0, GL_RGBA, surface->w, surface->h, 0,
// 	0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
//              Pixels type      Image bytes
// First RGBA: Desired color channels / Second RGBA: Image color channels
//                     ^- GL_RGB: jpg/jpeg, GL_RGBA: png
