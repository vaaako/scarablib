#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/image.hpp"
#include <SDL2/SDL_surface.h>
#include <GL/glew.h>

// Size needs to be multiple of 2
// 16x16, 32x32, 64x64, 128x128, 256x256 etc


// Texture object used for shapes (2D and 3D).
// It will allocate ~1mb of RAM for each loaded texture
class Texture {
	public:
		// Texture filtering type
		enum class Filter : uint32 {
			NEAREST =  GL_NEAREST,
			LINEAR = GL_LINEAR
		};

		// Texture wrapping type
		enum class Wrap : uint32 {
			REPEAT = GL_REPEAT,
			MIRRORED = GL_MIRRORED_REPEAT,
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		};

		// This will make a empty solid color texture.
		// Do not use this if you dont know what are you doing
		Texture(const Color& color) noexcept;

		// Constructor to create a texture from a file path with NEAREST filtering and REPEAT wrapping.
		// `wrap`: Wrap type. Optional. Default: Texture::Wrap::REPEAT.
		// You can also flip the texture vertically or horizontally
		Texture(const char* path, const bool flip_vertically = false, const bool flip_horizontally = false);

		// Uses pre-defined data to make a texture.
		Texture(const void* data, const uint32 width, const uint32 height, const GLint internal_format, const GLenum format);

		~Texture() noexcept;

		// Updates texture (this does not changes size and pixel format)
		// void update_data(const void* data, const GLenum format);

		// Returns the id of the texture
		inline uint32 get_id() const noexcept {
			return this->id;
		}
		
		// Returns the internal format of the texture
		// inline GLenum get_type() const noexcept {
		// 	return GL_TEXTURE_2D;
		// }

		// Returns the width of the texture
		inline uint32 get_width() const noexcept {
			return this->width;
		}

		// Returns the height of the texture
		inline uint32 get_height() const noexcept {
			return this->height;
		}

		// Bind the texture for use in rendering
		inline void bind() const noexcept {
			glBindTexture(GL_TEXTURE_2D, this->id);
		}

		// Unbind the texture to stop using it in rendering
		inline void unbind() const noexcept {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Change the filtering mode
		void set_filter(const Texture::Filter filter) const noexcept;

		// Change the wrapping mode
		void set_wrap(const Texture::Wrap wrap) const noexcept;

		// Returns a default solid white texture
		static Texture& default_texture() noexcept {
			// I don't like data being statically allocated this way
			static Texture def_tex = Texture(Colors::WHITE);
			return def_tex;
		}

		// Extracts which color format a image has.
		// If `internal` is true, returns enum for internal format
		static GLuint extract_format(const Image& image, const bool internal);

	private:
		GLuint id;
		uint32 width;
		uint32 height;

};
