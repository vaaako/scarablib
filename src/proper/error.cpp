#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>


ScarabError::ScarabError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	int size = std::vsnprintf(nullptr, 0, fmt, args) + 1; // +1 for null terminator
	va_end(args);

	if(size <= 0) {
		throw std::runtime_error("Error formatting message");
	}


	// Format the string
	std::vector<char> buffer = std::vector<char>(static_cast<uint64>(size));
	va_start(args, fmt);
	std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
	va_end(args);

	// Since is a char*, copy memory to destination
	this->message = std::string(buffer.data());
}

