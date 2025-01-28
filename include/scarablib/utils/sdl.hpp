#pragma once

#include "scarablib/typedef.hpp"
#include <SDL2/SDL_video.h>

// Helper namespace with modules related to SDL. Used to help code the library
namespace SDLHelper {
	// Flips an image vertically.
	// Original code: https://stackoverflow.com/questions/65815332/flipping-a-surface-vertically-in-sdl2
	void flip_vertically(SDL_Surface* surface) noexcept;
	// Flips an image horizontally.
	// Original code: https://stackoverflow.com/questions/65815332/flipping-a-surface-vertically-in-sdl2
	void flip_horizontally(SDL_Surface* surface) noexcept;

	// Original code: https://stackoverflow.com/questions/30016083/sdl2-opengl-sdl2-ttf-displaying-text
	//
	// Convert an image to have a dimension divisible by two.
	// "So OpenGL requires that all textures have dimensions of Base2 on my system (2,4,16,32,64...)"
	uint32 power_two_floor(uint32 value) noexcept;

	// Create a surface to the correct size in RGB format, and copy the old image
	SDL_Surface* base2_surface(SDL_Surface* surface) noexcept;
};
