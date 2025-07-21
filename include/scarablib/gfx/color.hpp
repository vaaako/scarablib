#pragma once

#include <SDL2/SDL_pixels.h>
#include "scarablib/typedef.hpp"

enum class Colors : uint {
	WHITE    = 0xffffff,
	BLACK    = 0x000000,
	ALMOSTBLACK = 0x222222,

	YELLOW   = 0xffff00,
	ORANGE   = 0xf08000,
	BROWN    = 0x6f4e37,

	RED      = 0xff0000,
	GREEN    = 0x00ff00,
	BLUE     = 0x0000ff,

	PINK     = 0xff69b4,
	LIME     = 0x32CD32,
	CYAN     = 0x40e0d0,

	WINE     = 0x811331,
	MOSS     = 0x06402b,
	DEEPBLUE = 0x0047ab,

	PURPLE   = 0x5d3fd3,

	MAGENTA     = 0xca1773,
	HOTPINK     = 0xff00da,
	CHIROYELLOW = 0xffe5b4
};

// This struct encapsulates the color data with red, green, blue, and alpha components,
// providing methods for conversion, comparison, and creation from various formats.
struct Color {
	uint8 red = 255;
	uint8 green = 255;
	uint8 blue = 255;
	uint8 alpha = 255;

	// Default constructor initializes the color to white
	constexpr Color() noexcept = default;

	// Initializes the color using an enum value from Colors
	constexpr Color(const Colors color) noexcept {
		const int hex_value = static_cast<int>(color);

		this->red = static_cast<uint8>((hex_value >> 16) & 0xFF);  // Extract red (bits 24-31)
		this->green = static_cast<uint8>((hex_value >> 8) & 0xFF); // Extract green (bits 16-23)
		this->blue = static_cast<uint8>(hex_value & 0xFF);         // Extract blue (bits 8-15)

		this->alpha = 255;
	}

	// Initializes the color using a hex value
	constexpr Color(const uint32 hex_value) noexcept {
		this->red = static_cast<uint8>((hex_value >> 16) & 0xFF);  // Extract red (bits 24-31)
		this->green = static_cast<uint8>((hex_value >> 8) & 0xFF); // Extract green (bits 16-23)
		this->blue = static_cast<uint8>(hex_value & 0xFF);         // Extract blue (bits 8-15)

		this->alpha = 255; // Default alpha to 255

		// Check for alpha
		if((hex_value >> 24) & 0xFF) {
			this->alpha = static_cast<uint8>((hex_value >> 24) & 0xFF); // Extract alpha (bits 32-39)
		}
	}

	// Initializes the color with specific RGBA values
	constexpr Color(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha) noexcept
		: red(red), green(green), blue(blue), alpha(alpha) {}

	Color(const Color&) noexcept = default;
	Color& operator=(const Color&) noexcept = default;
	Color(Color&&) noexcept = default;
	Color& operator=(Color&&) noexcept = default;

	// Normalize color values
	inline const vec4<float> normalize() const noexcept {
		return {
			this->red   / 255.0f,
			this->green / 255.0f,
			this->blue  / 255.0f,
			this->alpha / 255.0f
		};
	}

	// Returns the distance between two colors.
	// The smaller the distance, the closer the colors are
	float distance_to(const Color& other) const noexcept;

	// Blend with another color
	// constexpr void blend(const Color& other, const float t) noexcept;

	constexpr bool operator==(const Color& other) const noexcept {
		return (this->red == other.red)
				&& (this->green == other.green)
				&& (this->blue == other.blue)
				&& (this->alpha == other.alpha);
	}

	constexpr bool operator!=(const Color& other) const noexcept {
		return !(*this == other);
	}

	constexpr Color& operator=(const uint8 scalar) noexcept {
		this->red = scalar;
		this->green = scalar;
		this->blue = scalar;
		this->alpha = scalar;
		return *this;
	}
};
