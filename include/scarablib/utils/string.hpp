#pragma once

#include <cctype>
#include <string>

// Helper namespace with methods related to string manipulation
namespace StringHelper {
	// Get the file extension from a string
	std::string file_extension(const std::string& filename);

	// Get the file extension from char*
	inline std::string file_extension(const char* filename) {
		return StringHelper::file_extension(std::string(filename));
	}
}
