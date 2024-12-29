#include "scarablib/utils/string.hpp"

#include "scarablib/typedef.hpp"
#include <algorithm>

std::string StringHelper::file_extension(const std::string& filename) {
	// Find the last position of '.' in filename
	size_t dot_position = filename.find_last_of(".");
	std::string ext = (dot_position != std::string::npos)
		? filename.substr(dot_position + 1) // Extract and return extension
		: ""; // Return an empty string if period is not found

	// To lower case before returning
	std::transform(ext.begin(), ext.end(), ext.begin(), [](uint8 c) {
		return std::tolower(c);
	});
	return ext;
}
