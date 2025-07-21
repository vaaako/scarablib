#pragma once

#include "scarablib/gfx/texturebase.hpp"
#include "scarablib/typedef.hpp"
#include <GL/glew.h>
#include <vector>

// WARNING: NOT FINISHED. STILL WORKING ON IT

// This class represents a texture array.
// A texture array is a texture that contains multiple images inside
class TextureArray : public TextureBase {
	public:
		struct Config {
			const char* path;
			bool flip_vertically = false;
			bool flip_horizontally = false;
		};

		// Creates a texture array. All textures added to the array must have the same dimensions.
		// - `width`: Width of all textures
		// - `height`: Height of all textures
		// - `max_textures`: Desired limit of textures. Automatically clamped to 0-2048
		// - `channels`: (Default: 4) Number of channels
		//    + 1: Grayscale
		//    + 3: RGB (e.g. JPEG or PNG without alpha)
		//    + 4: RGBA (e.g. PNG with alpha)
		TextureArray(const uint16 width, const uint16 height, const uint16 max_textures, const uint8 channels = 4);
		~TextureArray() noexcept = default;

		// Add a new texture to the texture array and returns its index.
		// The texture must have the same width and height as the others
		uint16 add_texture(const char* path, const bool flip_vertically = false, const bool flip_horizontally = false);

		// Add new textures to the texture array and returns index of added texture.
		// All the textures must have the same width and height
		uint16 add_textures(const std::vector<TextureArray::Config>& paths);

		// Returns the current number of textures in the array
		inline uint32 get_num_textures() const noexcept {
			return this->num_textures;
		}

		// Returns the limit of textures that can be added
		inline uint32 get_max_textures() const noexcept {
			return this->max_textures;
		}

		// Generates mipmaps for all textures.
		// You must call this after adding all desired textures
		inline void generate_mipmap() const noexcept {
			this->bind();
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			this->unbind();
		}

	private:
		uint16 num_textures = 0; // Current number of textures
		uint16 max_textures;     // Limit of textures
		uint8 channels;          // Desired number of channels
};
