#pragma once

#include <SDL2/SDL_mixer.h>
#include "scarablib/typedef.hpp"

// This is a interface class and should not be used by itself
struct IAudio {
	// Path loaded
	const char* path;
	// Max volume possible
	const uint8 max_volume = 128;
	// Current volume
	uint8 volume = MIX_MAX_VOLUME;

	// This is a interface class and should not be used by itself
	IAudio(const char* path);
	// virtual ~Audio() = 0;

	virtual inline void play(const int8 loops = 0) const noexcept = 0;
	virtual inline void set_volume(const uint8 volume) noexcept = 0;

	virtual inline bool isplaying() const noexcept = 0;
	virtual inline bool ispaused() const noexcept = 0;
	virtual inline void pause() const noexcept = 0;
	virtual inline void resume() const noexcept = 0;
};

