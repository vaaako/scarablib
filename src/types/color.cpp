#include "scarablib/types/color.hpp"

// Returns the distance between two colors.
// The smaller the distance, the closer the colors are
float Color::distance_to(const Color& other) const noexcept {
	const int16 dr = this->red - other.red;
	const int16 dg = this->green - other.green;
	const int16 db = this->blue - other.blue;
	return std::sqrtf((float)(dr * dr + dg * dg + db * db));
}

// constexpr void Color::blend(const Color& other, const float t) noexcept {
// 	this->red   = static_cast<uint8_t>(this->red   * (1 - t) + other.red   * t);
// 	this->green = static_cast<uint8_t>(this->green * (1 - t) + other.green * t);
// 	this->blue  = static_cast<uint8_t>(this->blue  * (1 - t) + other.blue  * t);
// 	this->alpha = static_cast<uint8_t>(this->alpha * (1 - t) + other.alpha * t);
// }

