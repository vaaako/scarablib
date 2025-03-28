#pragma once

#include "scarablib/typedef.hpp"

// WARNING: SDL_mixer is a pain to use.
// So i am moving all method from header file, so i can easily share the library without having to include SDL_mixer.h

// This is a interface class and should not be used by itself
struct IAudio {
	// Max volume possible
	static constexpr uint8 MAX_VOLUME = 128;

	// Path loaded
	const char* path;
	// Current volume
	uint8 volume = this->MAX_VOLUME;

	// This is a interface class and should not be used by itself
	IAudio(const char* path);
	~IAudio() = default;

	virtual void play(const int8 loops = 0) const noexcept = 0;
	virtual void set_volume(const uint8 volume) noexcept = 0;

	virtual bool isplaying() const noexcept = 0;
	virtual bool ispaused() const noexcept = 0;
	virtual void pause() const noexcept = 0;
	virtual void resume() const noexcept = 0;
};

