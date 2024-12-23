#pragma once

#include "scarablib/typedef.hpp"
#include <SDL2/SDL_surface.h>
#include <GL/glew.h>

// Size needs to be multiple of 2
// 16x16, 32x32, 64x64, 128x128, 256x256 etc

enum class TextureFilter : int {
	NEAREST =  GL_NEAREST,
	LINEAR = GL_LINEAR 
};

enum class TextureWrap : int {
	REPEAT = GL_REPEAT,
	MIRRORED = GL_MIRRORED_REPEAT,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	// CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};


// TODO: use this later
struct TextureConf {
	TextureFilter filter = TextureFilter::LINEAR;
	TextureWrap wrap = TextureWrap::REPEAT;
	GLenum format = GL_RGBA;
	GLuint type = GL_TEXTURE_2D;
};

// Texture object used for shapes (2D and 3D).
// It will allocate ~1mb of RAM for each loaded texture
struct Texture {
	// This will make a empty solid white texture
	Texture();

	// Constructor to create a texture from a file path
	// Optionally sets the filtering and wrapping methods
	Texture(const char* path, const TextureFilter filter = TextureFilter::LINEAR, const TextureWrap wrap = TextureWrap::REPEAT);

	// Uses pre-defined data to make a texture
	// Texture(const void* data, const uint32 width, const uint32 height, const GLenum format);
	Texture(const void* data, const uint32 width, const uint32 height, const GLint internal_format, const GLenum format);

	// Default copy
	Texture(const Texture&) = default;
	Texture& operator=(const Texture& other) = default;

	// NOTE: I am not using default keyword just because of OpenGL ID, to avoid double deletion or any trouble with it 

	// Move
	Texture(Texture&& other) noexcept {
		this->id = other.id;
		this->format = other.format;
		this->width = other.width;
		this->height = other.height;
		this->tex_type = other.tex_type;
		other.id = 0;
	}
	Texture& operator=(Texture&& other) noexcept {
		if(this != &other) {
			// Delete current
			glDeleteTextures(1, &this->id);
			this->id = other.id;
			this->format = other.format;
			this->width = other.width;
			this->height = other.height;
			this->tex_type = other.tex_type;
			other.id = 0;
		}
		return *this;
	}

	~Texture();

	// Updates texture (this does not changes size and pixel format)
	void update_data(const void* data, const GLenum format);

	// Texture's width
	inline uint32 get_width() const {
		return this->width;
	}

	// Texture's height
	inline uint32 get_height() const {
		return this->width;
	}

	// Bind the texture for use in rendering
	inline void bind() const {
		glBindTexture(this->tex_type, this->id);
	}

	// Unbind the texture to stop using it in rendering
	inline void unbind() const {
		glBindTexture(this->tex_type, 0);
	}

	inline GLuint get_id() const {
		return this->id;
	}

	private:
		GLuint id;
		GLenum format;
		GLint width;
		GLint height;
		GLuint tex_type = GL_TEXTURE_2D; // Type of texture (2D by default)

		// Texture atlas
		// GLuint tex_type = GL_TEXTURE_2D_ARRAY;
};
