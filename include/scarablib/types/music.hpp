#pragma once

#include "scarablib/types/Iaudio.hpp"

// WARNING: SDL_mixer is a pain to use.
// So i am moving all method from header file, so i can easily share the library without having to include SDL_mixer.h

// Music object.
// Only one music can play at a time
struct Music : public IAudio {
	// What type is the music buffer
	enum Type {
		NONE,
		CMD,
		WAV,
		MOD,
		MID,
		OGG,
		MP3,
		MP3_MAD,
		FLAC,
		MODPLUG
	};

	enum class Fading : uint8 {
		NOT_FADING,
		FADING_OUT,
		FADING_IN
	};

	// Find out the format of a mixer music.
	Music::Type type;
	// Music duration in seconds
	// To convert to milliseconds, multiply by 1000.0.  
	double duration;

	Music(const char* path);
	~Music() noexcept;

	// Play the audio.
	// loops: Repeat by x times. -1 to repeat forever (~65000 times)
	void play(const int8 loops = 0) const noexcept override;
	// Play the music like `play` but with a fade in.
	// This will start the new music playing.
	// but will start the music playing at silence and fade in to its normal volume over the specified number of milliseconds
	// loops: Repeat by x times. -1 to repeat forever
	void play_fadein(const uint32 ms, const int8 loops = 0) const noexcept;
	// This will begin the music fading from its current time for `ms` milliseconds.
	// The fade out starts when is called
	void fade_out(const uint32 ms) const noexcept;
	// Set the volume for the audio (max volume: 128).
	// Values greater than max_volume are clamped to max_volume
	void set_volume(const uint8 volume) noexcept override;
	// Set the maximum volume for all channels
	void master_volume(const uint8 volume) noexcept;
	// Check if sound is playing
	bool isplaying() const noexcept override;
	// Check if sound is paused
	bool ispaused() const noexcept override;
	// Pause current sound
	void pause() const noexcept override;
	// Resume paused sound
	void resume() const noexcept override;
	// Re-play the music as it just started
	void rewind() noexcept;


	private:
		// SDL_mixer object
		void* music = nullptr; // Mix_Music*
};
