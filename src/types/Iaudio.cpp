#include "scarablib/types/Iaudio.hpp"
#include "scarablib/utils/string.hpp"
#include "scarablib/proper/error.hpp"
#include <SDL2/SDL.h>

IAudio::IAudio(const char* path) {
	// Check if extension is supported
	const std::string extension = StringHelper::file_extension(path);
	if(!(extension == "wav" || extension == "ogg" || extension == "flac" || extension == "mp3")) {
		SDL_Quit();
		Mix_CloseAudio();
		throw ScarabError("Audio format is not supported (%s)\nSupported formats are: WAV, MP3, OGG and FLAC", path);
	}
}

