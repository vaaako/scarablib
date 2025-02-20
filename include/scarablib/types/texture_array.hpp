#pragma once

#include "scarablib/typedef.hpp"
#include <GL/glew.h>
#include <vector>

// This class represents a texture array.
// A texture array is a texture that contains multiple images inside.  
// This is useful for animations or 3D models
struct TextureArray {
	struct Config {
		const char* path;
		bool flip_vertically = false;
		bool flip_horizontally = false;
	};

	// Creates a texture array, useful for animations or more complex models.
	// Takes a vector of configuration for each texture.
	// The configuration will be applied respectively to each texture.
	// NOTE: The size of all textures will be the same as the first texture
	TextureArray(const std::vector<TextureArray::Config>& textures);
	// TextureArray(const std::vector<const char*> paths);

	// Loads all images from a directory and stores them as a texture array.
	// The textures in the array will be ordered alphabetically by filename.
	// NOTE: The size of all textures will be the same as the first texture
	// TextureArray(const char* dir_path);

	// Disable Copying
	TextureArray(const TextureArray&) noexcept = delete;
	TextureArray& operator=(const TextureArray& other) noexcept = delete;

	// Disable Moving
	TextureArray(TextureArray&&) noexcept = delete;
	TextureArray& operator=(TextureArray&&) noexcept = delete;

	// Returns the id of the texture
	inline uint32 get_id() const noexcept {
		return this->id;
	}

	// Bind the texture for use in rendering.
	// Unit corresponds to the texture id in array
	inline void bind(const uint32 unit = 0) const noexcept {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	}

	// Unbind the texture to stop using it in rendering
	inline void unbind() const noexcept {
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	~TextureArray() noexcept;

	private:
		GLuint id;
		uint32 width;
		uint32 height;

		// Helper to load all textures on all constructors
		void load_all_tex(const std::vector<const char*> paths);
};
