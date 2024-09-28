#include "scarablib/proper/error.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <vector>


ScarabError::ScarabError(const char* fmt, ...) {
	std::vector<char> buffer = std::vector<char>(1024);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer.data(), buffer.size(), fmt, args); // Format the string
	va_end(args);

	// Since is a char*, copy memory to destination
	this->message = new char[buffer.size()];
	std::strcpy(this->message, buffer.data());
}

ScarabError::~ScarabError() {
	delete[] this->message;
}
