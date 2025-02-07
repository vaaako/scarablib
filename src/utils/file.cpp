#include "scarablib/utils/file.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ios>

std::string FileHelper::read_file(const std::string& path) {
	std::ifstream file = std::ifstream(path);
	if(!file.is_open()) {
		throw ScarabError("Could not open file %s", path.c_str());
	}

	// Prepare buffer
	std::string buffer;
	file.seekg(0, std::ios::end);

	const int64 size = file.tellg();
	buffer.resize((size_t)size);

	// Read
	file.seekg(0, std::ios::beg);
	file.read(buffer.data(), size);

	return buffer;
}

const std::vector<std::string> FileHelper::list_files(const char* path, const bool sort) {
	if(!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
		return std::vector<std::string>();
	}

	std::vector<std::string> result;

	for(const auto& entry : std::filesystem::directory_iterator(path)) {
		if(std::filesystem::is_regular_file(entry.path())) {
			result.push_back(entry.path().string());
		}
	}

	// Sort the file paths alphabetically
	if(sort) {
		std::sort(result.begin(), result.end());
	}

	return result;
}
