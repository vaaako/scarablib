#include "scarablib/types/sound.hpp"
#include "scarablib/proper/error.hpp"
#include <SDL2/SDL.h>

Sound::Sound(const char* path) : Audio(path) {
	// Load audio
	this->sound = Mix_LoadWAV(path);
	if (this->sound == NULL) {
		throw ScarabError("Sound \"%s\" was not found", path);
	}
}

Sound::~Sound() {
	Mix_FreeChunk(this->sound);
	Mix_CloseAudio();
}

