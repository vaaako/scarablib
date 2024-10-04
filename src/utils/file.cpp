#include "scarablib/utils/file.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <fstream>
#include <ios>

// char* FileHelper::read_file(const std::string& path) {
// 	std::ifstream file = std::ifstream(path);
//
// 	if(!file.is_open()) {
// 		throw ScarabError("Could not open file %s", path.c_str());
// 	}
//
// 	file.seekg(0, std::ios::end);
//
// 	// Allocate memory for reading file
// 	const int64 size = file.tellg(); // Get file size
// 	char* buffer = new char[static_cast<uint64>(size+ 1)]; // +1 for terminator character
//
// 	// Read file
// 	file.seekg(0, std::ios::beg);
// 	file.read(buffer, size); // Append to buffer
// 	buffer[size] = '\0'; // Append terminator character
// 	file.close();
//
// 	// delete []buffer;
// 	return buffer;
// }
//

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
