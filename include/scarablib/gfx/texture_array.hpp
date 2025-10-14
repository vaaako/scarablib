#pragma once

#include "scarablib/gfx/texturebase.hpp"
#include "scarablib/typedef.hpp"
#include <vector>

// WARNING: NOT FINISHED. STILL WORKING ON IT

// This class represents a texture array.
// A texture array is a texture that contains multiple images inside
class TextureArray : public TextureBase {
	public:
		// Current texture index being used
		uint16 texture_index = 0;

		// Config struct used to create multiple layers
		struct Layer {
			const char* path;
			bool flip_vertically = false;
			bool flip_horizontally = false;
		};

		// Creates a texture array. All layers added to the array must have the same dimensions.
		// - `width`: Width of all layers
		// - `height`: Height of all layers
		// - `max_layers`: Desired limit of layers. Automatically clamped to 0-2048
		// - `channels`: (Default: 4) Number of channels
		//    + 1: Grayscale
		//    + 3: RGB (e.g. JPEG or PNG without alpha)
		//    + 4: RGBA (e.g. PNG with alpha)
		TextureArray(const uint16 width, const uint16 height, const uint16 max_layers, const uint8 channels = 4);

		~TextureArray() noexcept = default;

		// Adds or replaces a layer of the texture array and returns its index.
		// All layers must have the same width and height
		uint16 add_texture(const char* path, const bool flip_vertically = false, const bool flip_horizontally = false, const int layer = -1);

		// Adds or replaces a layer of the texture array and returns its index.
		// All layers must have the same width and height
		uint16 add_textures(const std::vector<TextureArray::Layer>& paths);

		// Returns the current number of layers in the array
		inline uint32 get_num_layers() const noexcept {
			return this->num_layers;
		}

		// Returns the limit of layers that can be added
		inline uint32 get_max_layers() const noexcept {
			return this->max_layers;
		}

		// Generates mipmaps for all textures.
		// You must call this after adding all desired textures
		inline void generate_mipmap() const noexcept {
		#if !defined(BUILD_OPGL30)
			glGenerateTextureMipmap(this->id);
		#else
			this->bind();
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			this->unbind();
		#endif
		}
		// NOTE: here and not in TextureBase because only TextureArray needs this

	private:
		uint16 num_layers = 0; // Current number of layers
		uint16 next_layer = 0; // Next layer number to add
		uint16 max_layers;     // Limit of layers
		uint8 channels;        // Desired number of channels
};
