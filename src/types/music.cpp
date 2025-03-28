#include "scarablib/types/music.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include <SDL2/SDL_mixer.h>

// Path validation is done in IAudio
Music::Music(const char* path)
	: IAudio(path) {
	// Load audio
	this->music = Mix_LoadMUS(path);
	if (this->music == NULL) {
		throw ScarabError("Music \"%s\" was not found", path);
	}

	this->type     = (Music::Type)Mix_GetMusicType((Mix_Music*)this->music);
	this->duration = Mix_MusicDuration((Mix_Music*)this->music);
}

Music::~Music() noexcept {
	Mix_FreeMusic((Mix_Music*)this->music);
	Mix_CloseAudio();
}

void Music::play(const int8 loops) const noexcept {
	Mix_PlayMusic((Mix_Music*)this->music, loops);
}

void Music::play_fadein(const uint32 ms, const int8 loops) const noexcept {
	Mix_FadeInMusic((Mix_Music*)this->music, loops, (int)ms);
}

void Music::fade_out(const uint32 ms) const noexcept {
	Mix_FadeOutMusic((int)ms);
}

void Music::set_volume(const uint8 volume) noexcept {
	this->volume = volume;
	Mix_VolumeMusic(volume);
}

bool Music::isplaying() const noexcept {
	return Mix_PlayingMusic() == 1;
}

bool Music::ispaused() const noexcept {
	return Mix_PausedMusic() == 1;
}

void Music::pause() const noexcept {
	if(Mix_Playing(-1) == 0) {
		LOG_ERROR("Music is not playing");
		return;
	}
	Mix_PauseMusic();
}

void Music::resume() const noexcept {
	if(Mix_Paused(-1) == 0) {
		LOG_ERROR("Music is not playing");
		return;
	}
	Mix_ResumeMusic();
}

Music::Fading fading_status() noexcept {
	return (Music::Fading)Mix_FadingMusic();
}

void Music::rewind() noexcept {
	Mix_RewindMusic();
}
