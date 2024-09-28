#pragma once

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


// Texture object used for shapes (2D and 3D)
struct Texture {
	// Constructor to create a texture from a file path
	// Optionally sets the filtering and wrapping methods
	Texture(const char* path, const TextureFilter filter = TextureFilter::LINEAR, const TextureWrap wrap = TextureWrap::REPEAT);
	Texture(const SDL_Surface* surface, const TextureFilter filter = TextureFilter::LINEAR);
	~Texture();

	// Bind the texture for use in rendering
	inline void bind() const {
		glBindTexture(this->tex_type, this->id);
	}

	// Unbind the texture to stop using it in rendering
	inline void unbind() const {
		glBindTexture(this->tex_type, 0);
	}

private:
	GLuint id;
	GLuint tex_type = GL_TEXTURE_2D; // Type of texture (2D by default)

	// Optional: Uncomment for using texture atlases
	// GLuint tex_type = GL_TEXTURE_2D_ARRAY;
};
