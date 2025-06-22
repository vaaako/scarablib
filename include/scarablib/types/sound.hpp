#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/types/Iaudio.hpp"

// WARNING: SDL_mixer is a pain to use.
// So i am moving all method from header file, so i can easily share the library without having to include SDL_mixer.h

// SFX object.
// Multiple SFX can play at the same time
struct Sound : public IAudio {
	// Makes a sound object, used for sfx
	Sound(const char* path);
	~Sound() noexcept;

	// Play the audio.
	// loops: Repeat by x times. -1 to repeat forever (~65000 times)
	void play(const int8 loops = 0) const noexcept override;
	// Set the volume for the audio (max volume: 128).
	// Values greater than max_volume are clamped to max_volume
	void set_volume(const uint8 volume) noexcept override;
	// Set the maximum volume for all channels
	void master_volume(const uint8 volume) noexcept;
	// Check if sound is playing
	bool isplaying() const noexcept override;
	// Check if sound is paused
	bool ispaused() const noexcept override;
	// Pause current sound.
	// Does nothing if sound is not playing
	void pause() const noexcept override;
	// Resume paused sound.
	// Does nothing if sound is not paused
	void resume() const noexcept override;

	private:
		// SDL_mixer object
		void* sound = nullptr; // Mix_Chunk*
};
