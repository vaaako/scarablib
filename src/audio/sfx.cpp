#include "scarablib/audio/sfx.hpp"
#include "scarablib/proper/error.hpp"
#include <SDL2/SDL_mixer.h>

// Path validation is done in IAudio
Sfx::Sfx(const char* path) : IAudio(path) {
	// Load audio
	this->sfx = Mix_LoadWAV(path);
	if (this->sfx == NULL) {
		throw ScarabError("Audio \"%s\" was not found", path);
	}
}

Sfx::~Sfx() noexcept {
	Mix_FreeChunk((Mix_Chunk*)this->sfx);
	Mix_CloseAudio();
}

void Sfx::play(const int8 loops) const noexcept {
	Mix_PlayChannel(-1, (Mix_Chunk*)this->sfx, (loops > 0) ? loops - 1 : loops); // for some reason
}

void Sfx::set_volume(const uint8 volume) noexcept {
	this->volume = volume;
	Mix_VolumeChunk((Mix_Chunk*)this->sfx, volume);
}

void Sfx::master_volume(const uint8 volume) noexcept {
	this->volume = volume;
	Mix_MasterVolume(volume);
}

bool Sfx::isplaying() const noexcept {
	return (Mix_Playing(-1) == 1);
}

bool Sfx::ispaused() const noexcept {
	return Mix_Paused(-1) == 1;
}

void Sfx::pause() const noexcept {
	if(Mix_Playing(-1) == 0) {
		return;
	}
	Mix_Pause(-1);

}

void Sfx::resume() const noexcept {
	if(Mix_Paused(-1) == 0) {
		return;
	}
	Mix_Resume(-1);
}

