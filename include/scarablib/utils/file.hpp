#pragma once

#include <filesystem>

// NOTE -- Need to be a macro because of __FILE__
// Get current file (implemented macro) absolute path
#define SOURCE_DIR std::filesystem::path(__FILE__).parent_path().string()

namespace FileHelper {
	// Return the content of a file
	std::string read_file(const std::string& path);

	// Get the current file static path
	inline std::filesystem::path current_path() {
		return std::filesystem::current_path();
	}
};
