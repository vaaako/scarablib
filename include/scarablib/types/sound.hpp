#pragma once

#include <SDL2/SDL_mixer.h>
#include "scarablib/typedef.hpp"
#include "scarablib/types/Iaudio.hpp"

// SFX object
struct Sound : public IAudio {
	// Makes a sound object, used for sfx
	Sound(const char* path);
	~Sound() noexcept;

	// Play the audio
	//
	// If `loops` is greater than zero, loop the sound that many times. If `loops` is -1, loop "infinitely" (~65000 times)
	inline void play(const int8 loops = 0) const noexcept override {
		Mix_PlayChannel(-1, this->sound, (loops > 0) ? loops - 1 : loops);
	}

	// Set the volume for the audio (max volume: 128)
	//
	// Values greater than max_volume are clamped to max_volume
	inline void set_volume(const uint8 volume) noexcept override {
		// Set volume
		this->volume = volume;
		Mix_VolumeChunk(this->sound, volume);
	}


	// Set the maximum volume for all channels
	inline void master_volume(const uint8 volume) noexcept {
		Mix_MasterVolume(volume);
	}

	// Query if the audio is playing
	inline bool isplaying() const noexcept override {
		return (Mix_Playing(-1) != 0);
	}

	// Query if sound is paused
	inline bool ispaused() const noexcept override {
		return Mix_Paused(-1) != 0;
	}

	inline void pause() const noexcept override {
		Mix_Pause(-1);

	}

	inline void resume() const noexcept override {
		Mix_Resume(-1);
	}

	private:
		// SDL_mixer object
		Mix_Chunk* sound = nullptr;
};
