#pragma once

#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/image.hpp"
#include <SDL2/SDL_surface.h>
#include <GL/glew.h>

// Size needs to be multiple of 2
// 16x16, 32x32, 64x64, 128x128, 256x256 etc


// Texture object used for shapes (2D and 3D).
// It will allocate ~1mb of RAM for each loaded texture
struct Texture {
	enum class Filter : uint32 {
		NEAREST =  GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum class Wrap : uint32 {
		REPEAT = GL_REPEAT,
		MIRRORED = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		// CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	// enum class Type : uint32 {
	// 	TEXTURE_2D = GL_TEXTURE_2D,
	// 	TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY
	// };

	struct Config {
		const char* path;

		Texture::Filter filter = Texture::Filter::NEAREST;
		Texture::Wrap wrap = Texture::Wrap::REPEAT;
		// Texture::Type type = Texture::Type::TEXTURE_2D;

		bool flip_vertically = false;
		bool flip_horizontally = false;
	};

	// This will make a empty solid color texture.
	// Do not use this if you dont know what are you doing
	Texture(const Color& color) noexcept;

	~Texture() noexcept;

	// Constructor to create a texture from a file path.
	// Optionally sets the filtering and wrapping methods
	Texture(const Texture::Config& config);

	// Uses pre-defined data to make a texture.
	Texture(const void* data, const uint32 width, const uint32 height, const GLint internal_format, const GLenum format);
	// Texture(const void* data, const uint32 width, const uint32 height, const GLenum format);

	// Disable Copy and Moving
	Texture(const Texture&) noexcept = delete;
	Texture& operator=(const Texture& other) noexcept = delete;
	Texture(Texture&&) noexcept = delete;
	Texture& operator=(Texture&&) noexcept = delete;

	// Updates texture (this does not changes size and pixel format)
	// void update_data(const void* data, const GLenum format);

	// Returns the id of the texture
	inline uint32 get_id() const noexcept {
		return this->id;
	}

	// Bind the texture for use in rendering
	inline void bind() const noexcept {
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	// Unbind the texture to stop using it in rendering
	inline void unbind() const noexcept {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// NOTE: I wish this was a member, but i would have to initialize in window.hpp
	// because i cant make a texture if the opengl context isnt ready yet
	// so lazy load...
	static Texture& default_texture() noexcept {
		// I don't like data being statically allocated but whatever
		static Texture def_tex = Texture(Colors::WHITE); // Make solid white texture
		return def_tex;
	}

	static GLuint extract_format(const Image& image) {
		switch (image.nr_channels) {
			case 1:
				return GL_RED;
				break;
			case 3:
				return GL_RGB;
				break;
			case 4:
				return GL_RGBA;
				break;
			default:
				throw ScarabError("Failed to load texture (%s). Unsupported format: %d channels", image.path, image.nr_channels);
		}
	}

	static GLuint extract_internal_format(const Image& image) {
		switch (image.nr_channels) {
			case 1:
				return GL_R8;
				break;
			case 3:
				return GL_RGB8;
				break;
			case 4:
				return GL_RGBA8;
				break;
			default:
				throw ScarabError("Failed to load texture (%s). Unsupported format: %d channels", image.path, image.nr_channels);
		}
	}

	private:
		GLuint id;
		uint32 width;
		uint32 height;

};
