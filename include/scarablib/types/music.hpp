#pragma once

#include <SDL2/SDL_mixer.h>
#include "scarablib/types/Iaudio.hpp"

enum FadingStatus {
	NOT_FADING,
	FADING_OUT,
	FADING_IN
};

// Music object
struct Music : public IAudio {
	// Find out the format of a mixer music.  
	Mix_MusicType type;
	// Music duration in seconds
	double duration;

	// Makes a music object, used for music
	//
	// Only one music can play
	Music(const char* path);
	~Music() noexcept;


	// Play the music
	//
	// Only one music can play at a time
	//
	// loops: Repeat by x times. -1 to repeat forever
	inline void play(const int8 loops = 0) const noexcept override {
		Mix_PlayMusic(this->music, loops);
	}

	// Play the music like `play` but with a fade in
	//
	// This will start the new music playing, but will start the music playing at silence and fade in to its normal volume over the specified number of milliseconds
	//
	// loops: Repeat by x times. -1 to repeat forever
	inline void play_fadein(const uint ms, const int8 loops = 0) const noexcept {
		Mix_FadeInMusic(this->music, loops, (int)ms);
	}

	// Set music volume (max volume: 128)
	//
	// Values greater than max_volume are clamped to max_volume
	inline void set_volume(const uint8 volume) noexcept override {
		this->volume = volume;
		Mix_VolumeMusic(volume);
	}


	// This will begin the music fading from its current time for `ms` milliseconds
	//
	// The fade out starts when is called
	inline void fade_out(const uint ms) const noexcept {
		Mix_FadeOutMusic((int)ms);
	}


	// Query if the music is playing
	inline bool isplaying() const noexcept override {
		return (Mix_PlayingMusic() != 0);
	}

	// Query if the music is paused
	inline bool ispaused() const noexcept override {
		return Mix_PausedMusic() == 1;
	}

	// Pause the music stream
	inline void pause() const noexcept override {
		Mix_PauseMusic();
	}

	// Resume paused music
	inline void resume() const noexcept override {
		Mix_ResumeMusic();
	}

	// Get the current status of fading
	//
	// Three possible values are:
	// - `NOT_FADING`
	// - `FADING_OUT`
	// - `FADING_IN`
	inline FadingStatus fading_status() noexcept {
		return (FadingStatus)Mix_FadingMusic();
	}

	// Re-play the music
	inline void rewind() noexcept {
		Mix_RewindMusic();
	}

	private:
		// SDL_mixer object
		Mix_Music* music = nullptr;
};
