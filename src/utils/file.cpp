#include "scarablib/utils/file.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
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
