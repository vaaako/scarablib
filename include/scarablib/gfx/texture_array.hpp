#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/gfx/texture.hpp"
#include <GL/glew.h>
#include <vector>

// WARNING: NOT FINISHED. STILL WORKING ON IT

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
	// Takes the width and height of all textures and the limit of textures.
	// You can add textures later with TextureArray::add_texture.
	// The configuration will be applied respectively to each texture.
	// NOTE: Since you will manually add textures, remember to call `TextureArray::generate_mipmap` after adding all textures
	TextureArray(const uint32 width, const uint32 height, const uint32 num_textures, const Texture::Filter filter = Texture::Filter::NEAREST);

	// Creates a texture array, useful for animations or more complex models.
	// Takes a vector of configuration for each texture.
	// The configuration will be applied respectively to each texture.
	// num_textures is the number of textures in the array, if not specified it will be the number of textures in the vector.
	// NOTE: Mipmap is generated automatically if num_textures is not specified.
	// Remember to call `TextureArray::generate_mipmap` after adding all textures.
	// NOTE: The size of all textures will be the same as the first texture
	TextureArray(const std::vector<TextureArray::Config>& textures, const Texture::Filter filter = Texture::Filter::NEAREST, const uint32 num_textures = 0);

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

	~TextureArray() noexcept;

	// Returns the id of the texture
	inline uint32 get_id() const noexcept {
		return this->id;
	}

	// Returns the current index of textures in the array
	inline uint32 get_current_index() const noexcept {
		return static_cast<uint32>(this->cur_index);
	}

	// Returns the maximum number of textures that can be added
	inline uint32 get_max_textures() const noexcept {
		return this->max_textures;
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

	// Generates mipmaps for the texture.
	// You must call this after adding all desired textures
	inline void generate_mipmap() const noexcept {
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	// Add a new texture to the texture array.
	// All the textures must have the same width and height.
	// Remember to call `TextureArray::generate_mipmap` after adding all desired textures
	void add_texture(const TextureArray::Config& config);

	private:
		GLuint id;
		GLint width;
		GLint height;
		uint32 max_textures;
		GLint cur_index = 0; // Current Z-index of the latest added texture

		// Helper to load all textures on all constructors
		void load_all_tex(const std::vector<const char*> paths);
};
