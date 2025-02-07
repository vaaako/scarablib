#pragma once

#include "scarablib/typedef.hpp"
#include <GL/glew.h>
#include <string>
#include <vector>

// This class represents a texture array.
// A texture array is a texture that contains multiple images inside.  
// This is useful for animations or 3D models
struct TextureArray {
	// Creates a texture array, useful for animations or more complex models.
	// NOTE: The size of all textures will be the same as the first texture
	TextureArray(const std::vector<const char*> paths);

	// Loads all images from a directory and stores them as a texture array.
	// The textures in the array will be ordered alphabetically by filename.
	// NOTE: The size of all textures will be the same as the first texture
	TextureArray(const char* dir_path);

	~TextureArray() noexcept;

	private:
		const GLenum tex_type = GL_TEXTURE_2D_ARRAY;

		GLuint id;
		GLenum format;
		uint32 width;
		uint32 height;

		// Helper to load all textures on all constructors
		void load_all_tex(const std::vector<const char*> paths);
};
