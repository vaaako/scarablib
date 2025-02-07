#pragma once

#include <filesystem>
#include <vector>

// NOTE: Need to be a macro because of __FILE__
// Get current file absolute path (implemented macro)
#define THIS_FILE_DIR std::filesystem::path(__FILE__).parent_path().string()

namespace FileHelper {
	// Return the content of a file
	std::string read_file(const std::string& path);

	// Returns a vector containing all files inside a directory
	const std::vector<std::string> list_files(const char* path, const bool sort);

	// Returns the current static path of the file
	// For absolute path, use the macro THIS_FILE_DIR
	inline std::filesystem::path current_path() noexcept {
		return std::filesystem::current_path();
	}

	// Check if a path is a directory
	inline bool is_directory(const std::string& path) noexcept {
		return std::filesystem::is_directory(std::filesystem::path(path));
	}
};
