#include "scarablib/audio/Iaudio.hpp"
#include "scarablib/utils/string.hpp"
#include "scarablib/proper/error.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <array>

IAudio::IAudio(const char* path) {
	// Supported extensions lookup table
	constexpr std::array<const std::string_view, 5> supported_extensions = {
		"wav", "ogg", "flac", "mp3", "opus"
	};

	const std::string extension = StringHelper::file_extension(path);

	if(std::ranges::find(supported_extensions.begin(), supported_extensions.end(), extension) == supported_extensions.end()) {
		SDL_Quit();
		Mix_CloseAudio();
		throw ScarabError(
			"Audio format is not supported (%s)\nSupported formats are: WAV, MP3, OGG, OPUS and FLAC", 
			path
		);
	}
}

