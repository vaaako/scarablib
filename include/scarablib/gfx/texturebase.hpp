#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/window/window.hpp"
#include <algorithm>

// WARNING: Do not use this class directly
class TextureBase {
	public:
		// Texture filtering type
		enum class Filter : uint32 {
			NEAREST =  GL_NEAREST,
			LINEAR  = GL_LINEAR
		};

		// Texture wrapping type
		enum class Wrap : uint32 {
			REPEAT        = GL_REPEAT,
			MIRRORED      = GL_MIRRORED_REPEAT,
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		};

		// ID may be defined later or pass an existing one
		TextureBase(const uint16 width, const uint16 height, const uint32 id = 0) noexcept;
		~TextureBase() noexcept;

		// Returns the id of the texture
		inline constexpr uint32 get_id() const noexcept {
			return this->id;
		}

		// Returns the width of the texture
		inline constexpr uint16 get_width() const noexcept {
			return this->width;
		}

		// Returns the height of the texture
		inline constexpr uint16 get_height() const noexcept {
			return this->height;
		}

		// Bind the texture for use in rendering
		inline void bind(const uint8 unit = 0) const noexcept {
			glActiveTexture(GL_TEXTURE0 + std::clamp((int)unit, 0, 31));
			glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
		}

		// Unbind the texture to stop using it in rendering
		inline void unbind() const noexcept {
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}

		// Changes the filtering mode
		void set_filter(const TextureBase::Filter filter) const noexcept;

		// Changes the wrapping mode
		void set_wrap(const TextureBase::Wrap wrap) const noexcept;

		// Converts number of channels to enum.
		// If `internal` is true, returns enum for internal format
		// Returns 0 if invalid format.
		static uint32 extract_format(const uint8 num_channels, const bool internal);
	protected:
		uint32 id;
		uint16 width;
		uint16 height;
};
