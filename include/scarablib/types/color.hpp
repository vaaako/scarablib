#pragma once

#include <SDL2/SDL_pixels.h>
#include "scarablib/typedef.hpp"
#include "scarablib/proper/vector/vec3.hpp"
#include "scarablib/proper/vector/vec4.hpp"

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
	Color() = default;

	// Initializes the color using an enum value from Colors.
	// e.g., `Color color = Colors::MAGENTA;`
	Color(const Colors);

	// Initializes the color using a hex value.
	// The hex value should be in the format 0xRRGGBB or 0xAARRGGBB
	//
	// Example:
	// Color color1 = Color(0xFF5733);   // RGB (255, 87, 51) with alpha set to 255 .
	// Color color2 = Color(0x80FF5733); // RGBA (128, 255, 87, 51) with alpha set to 128
	Color(const int hex_value);

	// Initializes the color with specific RGBA values
	Color(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha = 255);

	// Copy constructor that initializes the color from another Color instance
	// e.g., `Color color = Color({ 202, 23, 115 });`
	Color(const Color& other) = default;


	// Check if all color components are equal to 0
	inline constexpr bool isempty() const {
		return (this->red + this->green + this->blue + this->alpha) == 0;
	}

	// Convert the color to a vec4<uint8>
	inline vec4<uint8> to_vec4() const {
		return vec4(this->red, this->green, this->blue, this->alpha);
	}

	// Convert the color to an `SDL_Color` object for use with SDL.
	inline SDL_Color to_sdl_color() const {
		return {
			this->red,
			this->green,
			this->blue,
			this->alpha
		};
	}

	// Convert color component values to a normalized range (0-1) for OpenGL
	inline const vec4<float> normalize() const {
		// This is not optimized and i have no idea of how to make this optmized
		return {
			this->red   / 255.0f,
			this->green / 255.0f,
			this->blue  / 255.0f,
			this->alpha / 255.0f
		};
	}

	bool operator==(const Color& other) const noexcept {
		return (this->red == other.red)
				&& (this->green == other.green)
				&& (this->blue == other.blue)
				&& (this->alpha == other.alpha);
	}

	bool operator!=(const Color& other) const noexcept {
		return !(*this == other);
	}

	Color& operator=(const Color& other) = default;

	// This can be used to create shades of gray
	Color& operator=(const uint8 scalar) noexcept {
		this->red = scalar;
		this->green = scalar;
		this->blue = scalar;
		this->alpha = scalar;
		return *this;
	}

	private:
};