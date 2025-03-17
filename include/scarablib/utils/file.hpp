#pragma once

#include "scarablib/typedef.hpp"
#include <filesystem>
#include <vector>

// NOTE: Need to be a macro because of __FILE__
// Get current file absolute path (implemented macro)
#define THIS_FILE_DIR std::filesystem::path(__FILE__).parent_path().string()

namespace FileHelper {
	// Return the content of a file.
	// Returns an empty string if the file does not exist
	std::string read_file(const std::string& path);

	// Return the content of a file as char*.
	// Returns nullptr if the file does not exist.
	// WARNING: delete the returned char*
	char* read_file_char(const std::string& path);

	// Return the content of a file.
	// Returns an empty vector if the file does not exist
	std::vector<uint8> read_binary_file(const char* path) noexcept;

	// Returns a vector containing all files inside a directory.
	// Returns an empty vector if the directory does not exist
	std::vector<std::string> list_files(const char* path, const bool sort);


	// Return the absolute path of the executable
	std::string executable_path();

	// Returns the current static path of the file
	// For absolute path, use the macro THIS_FILE_DIR
	inline std::filesystem::path current_path() noexcept {
		return std::filesystem::current_path();
	}

	// Check if a path is a directory
	inline bool is_directory(const std::string& path) noexcept {
		return std::filesystem::is_directory(std::filesystem::path(path));
	}

	inline bool file_exists(const std::string& path) noexcept {
		return std::filesystem::exists(path);
	}
};
