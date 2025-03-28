#include "scarablib/types/sound.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include <SDL2/SDL_mixer.h>

// Path validation is done in IAudio
Sound::Sound(const char* path) : IAudio(path) {
	// Load audio
	this->sound = Mix_LoadWAV(path);
	if (this->sound == NULL) {
		throw ScarabError("Sound \"%s\" was not found", path);
	}
}

Sound::~Sound() noexcept {
	Mix_FreeChunk((Mix_Chunk*)this->sound);
	Mix_CloseAudio();
}

void Sound::play(const int8 loops) const noexcept {
	Mix_PlayChannel(-1, (Mix_Chunk*)this->sound, (loops > 0) ? loops - 1 : loops); // for some reason
}

void Sound::set_volume(const uint8 volume) noexcept {
	this->volume = volume;
	Mix_VolumeChunk((Mix_Chunk*)this->sound, volume);
}

void Sound::master_volume(const uint8 volume) noexcept {
	this->volume = volume;
	Mix_MasterVolume(volume);
}

bool Sound::isplaying() const noexcept {
	return (Mix_Playing(-1) == 1);
}

bool Sound::ispaused() const noexcept {
	return Mix_Paused(-1) == 1;
}

void Sound::pause() const noexcept {
	if(Mix_Playing(-1) == 0) {
		LOG_ERROR("Sound is not playing");
		return;
	}
	Mix_Pause(-1);

}

void Sound::resume() const noexcept {
	if(Mix_Paused(-1) == 0) {
		LOG_ERROR("Sound is not paused");
		return;
	}
	Mix_Resume(-1);
}

