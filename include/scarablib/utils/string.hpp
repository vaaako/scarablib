#pragma once

#include <cctype>
#include <string>

// Helper namespace with methods related to string manipulation
namespace StringHelper {
	// Get the file extension from a string.
	// If not found, returns an empty string
	std::string file_extension(const std::string& filename) noexcept;

	// Get the file extension from char*.
	// If not found, returns an empty string
	inline std::string file_extension(const char* filename) noexcept {
		return StringHelper::file_extension(std::string(filename));
	}

	// Get the base directory from a path to a file.
	// If not found, returns "."
	std::string base_dir(const std::string& path) noexcept;

	// Get the base directory from a path to a file.
	// If not found, returns "."
	inline std::string base_dir(const char* path) noexcept {
		return StringHelper::base_dir(std::string(path));
	}
}
