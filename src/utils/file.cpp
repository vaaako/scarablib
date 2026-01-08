#include "scarablib/utils/file.hpp"
#include "scarablib/typedef.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ios>

#include <unistd.h> // readlink

std::string ScarabFile::read_file(const std::filesystem::path& path) noexcept {
	if(!ScarabFile::file_exists(path)) {
		return "";
	}
	std::ifstream file = std::ifstream(path);

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

char* ScarabFile::read_file_char(const std::filesystem::path& path) noexcept {
	if(!ScarabFile::file_exists(path)) {
		return nullptr;
	}

	std::string content = ScarabFile::read_file(path);
	size_t size = content.size();

	// Allocate memory for the content
	char* result = new char[size + 1];
	std::copy(content.begin(), content.end(), result);

	result[size] = '\0'; // Null-terminate the string
	return result;
}

std::vector<uint8> ScarabFile::read_binary_file(const std::filesystem::path& path) noexcept {
	if(!ScarabFile::file_exists(path)) {
		return {};
	}
	// Open in binary mode
	std::ifstream file = std::ifstream(path, std::ios::binary | std::ios::ate);

	// Get file size
	std::streamsize size = file.tellg();

	// Return to the beginning for reading
	file.seekg(0, std::ios::beg);

	// Create a buffer
	std::vector<uint8> buffer = std::vector<uint8>(static_cast<size_t>(size));

	// Read file into buffer
	if(!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
		return {};
	}

	return buffer;
}

std::vector<std::string> ScarabFile::list_files(const std::filesystem::path& path, const bool sort) noexcept {
	if(!ScarabFile::file_exists(path) || !ScarabFile::is_dir(path)) {
		return {};
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

std::string ScarabFile::executable_path() {
#ifdef _WIN32
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::string(buffer);
#else
	char buffer[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
	return std::string(buffer, static_cast<size_t>((count > 0) ? count : 0));
#endif
}

