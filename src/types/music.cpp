#include "scarablib/types/music.hpp"
#include "scarablib/proper/error.hpp"

Music::Music(const char* path) : IAudio(path) {
	// Load audio
	this->music = Mix_LoadMUS(path);
	if (this->music == NULL) {
		throw ScarabError("Music \"%s\" was not found", path);
	}

	this->type  = Mix_GetMusicType(this->music);
	this->duration = Mix_MusicDuration(this->music);
}

Music::~Music() {
	Mix_FreeMusic(this->music);
	Mix_CloseAudio();
}
