#include <scarablib/types/color.hpp>



Color::Color(const uint32 hex_value) {
	this->red = static_cast<uint8>((hex_value >> 16) & 0xFF);  // Extract red (bits 24-31)
	this->green = static_cast<uint8>((hex_value >> 8) & 0xFF); // Extract green (bits 16-23)
	this->blue = static_cast<uint8>(hex_value & 0xFF);         // Extract blue (bits 8-15)

	this->alpha = 255; // Default alpha to 255

	// Check includes alpha
	if ((hex_value >> 24) & 0xFF) {
		this->alpha = static_cast<uint8>((hex_value >> 24) & 0xFF); // Extract alpha (bits 32-39)
	}
}

Color::Color(const Colors color) : Color() {
	int hex_value = static_cast<int>(color);

	this->red = static_cast<uint8>((hex_value >> 16) & 0xFF);  // Extract red (bits 24-31)
	this->green = static_cast<uint8>((hex_value >> 8) & 0xFF); // Extract green (bits 16-23)
	this->blue = static_cast<uint8>(hex_value & 0xFF);         // Extract blue (bits 8-15)

	this->alpha = 255;
}

Color::Color(const uint8 red, const uint8 green, const uint8 blue, const uint8 alpha)
	: red(red), green(green), blue(blue), alpha(alpha) {
}



