#pragma once

#include <SDL2/SDL_pixels.h>
#include "scarablib/typedef.hpp"

enum class Colors : uint {
	WHITE    = 0xffffff,
	BLACK    = 0x000000,

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

	// Default constructor initializes the color to white (255, 255, 255, 255)
	constexpr Color() noexcept = default;

	// Initializes the color using an enum value from Colors.
	// e.g., `Color color = Colors::MAGENTA;`
	constexpr Color(const Colors color) noexcept : Color() {
		int hex_value = static_cast<int>(color);

		this->red = static_cast<uint8>((hex_value >> 16) & 0xFF);  // Extract red (bits 24-31)
		this->green = static_cast<uint8>((hex_value >> 8) & 0xFF); // Extract green (bits 16-23)
		this->blue = static_cast<uint8>(hex_value & 0xFF);         // Extract blue (bits 8-15)

		this->alpha = 255;
	}

	// Initializes the color using a hex value.
	// The hex value should be in the format 0xRRGGBB or 0xAARRGGBB
	//
	// Example:
	// Color color1 = Color(0xFF5733);   // RGB (255, 87, 51) with alpha set to 255 .
	// Color color2 = Color(0x80FF5733); // RGBA (128, 255, 87, 51) with alpha set to 128
	explicit constexpr Color(const uint32 hex_value) noexcept {
		this->red = static_cast<uint8>((hex_value >> 16) & 0xFF);  // Extract red (bits 24-31)
		this->green = static_cast<uint8>((hex_value >> 8) & 0xFF); // Extract green (bits 16-23)
		this->blue = static_cast<uint8>(hex_value & 0xFF);         // Extract blue (bits 8-15)

		this->alpha = 255; // Default alpha to 255

		// Check includes alpha
		if ((hex_value >> 24) & 0xFF) {
			this->alpha = static_cast<uint8>((hex_value >> 24) & 0xFF); // Extract alpha (bits 32-39)
		}
	}


	// Initializes the color with specific RGBA values
	inline constexpr Color(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha) noexcept
		: red(red), green(green), blue(blue), alpha(alpha) {}

	// Copy constructor that initializes the color from another Color instance
	// e.g., `Color color = Color({ 202, 23, 115 });`
	Color(const Color&) noexcept = default;
	Color& operator=(const Color&) noexcept = default;
	// Move
	Color(Color&&) noexcept = default;
	Color& operator=(Color&&) noexcept = default;

	// Check if all color components are equal to 0
	inline constexpr bool isempty() const noexcept {
		return (this->red == 0) && (this->green == 0) && (this->blue == 0) && (this->alpha == 0);
	}

	// Convert the color to a vec4 of any type T.
	inline vec4<float> to_vec4() const noexcept {
		return vec4<float>(this->red, this->green, this->blue, this->alpha);
	}

	// Convert color component values to a normalized range (0-1) for OpenGL
	inline const vec4<float> normalize() const noexcept {
		// This is not optimized and i have no idea of how to make this optmized
		// Multiplying by 0.004 gives an approximated result as dividing by 255
		return {
			this->red   * 0.004f,
			this->green * 0.004f,
			this->blue  * 0.004f,
			this->alpha * 0.004f
		};
	}

	// Blend current color with another color
	inline constexpr Color blend(const Color& other, float t) const noexcept {
		return Color(
			static_cast<uint8_t>(this->red   * (1 - t) + other.red   * t),
			static_cast<uint8_t>(this->green * (1 - t) + other.green * t),
			static_cast<uint8_t>(this->blue  * (1 - t) + other.blue  * t),
			static_cast<uint8_t>(this->alpha * (1 - t) + other.alpha * t)
		);
	}

	// Darken the current color using a value
	inline constexpr Color darken(const float factor) const noexcept {
		return Color(
			static_cast<uint8_t>(this->red   * factor),
			static_cast<uint8_t>(this->green * factor),
			static_cast<uint8_t>(this->blue  * factor),
			alpha
		);
	}

	// Lighten the current color using a value
	inline constexpr Color lighten(const float factor) const noexcept {
		return Color(
			static_cast<uint8_t>(this->red   + (255 - this->red)   * factor),
			static_cast<uint8_t>(this->green + (255 - this->green) * factor),
			static_cast<uint8_t>(this->blue  + (255 - this->blue)  * factor),
			alpha
		);
	}

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
