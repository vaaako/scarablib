#include "scarablib/utils/string.hpp"

std::string StringHelper::file_extension(const std::string& filename) {
	// Find the last position of '.' in filename
	size_t dot_position = filename.find_last_of(".");
	return (dot_position != std::string::npos)
		? filename.substr(dot_position + 1) // Extract and return extension
		: ""; // Return an empty string if period is not found
}

