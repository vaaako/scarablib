#include "scarablib/proper/log.hpp"

#include <cstdarg>
#include <ctime>
#include <sstream>
#include <vector>

std::string Log::get_date() {
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	std::ostringstream oss;
	oss << (localTime->tm_year + 1900) << '-'
		<< (localTime->tm_mon + 1) << '-'
		<< localTime->tm_mday << ' ';
	return oss.str();
}

std::string Log::get_time() {
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	std::ostringstream oss;
	oss << localTime->tm_hour << ':'
		<< localTime->tm_min << ':'
		<< localTime->tm_sec;
	return oss.str();
}

std::string Log::get_date_and_time() {
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	std::ostringstream oss;
	oss << (localTime->tm_year + 1900) << '-'
		<< (localTime->tm_mon + 1) << '-'
		<< localTime->tm_mday << ' '
		<< localTime->tm_hour << ':'
		<< localTime->tm_min << ':'
		<< localTime->tm_sec;
	return oss.str();
}


void Log::log(const char* fmt, ...) {
	std::vector<char> buffer = std::vector<char>(1024);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer.data(), buffer.size(), fmt, args); // Format the string

	std::printf("%s\n", buffer.data());
	va_end(args);
}

void Log::log(const char* level, const char* fmt, ...) {
	std::vector<char> buffer = std::vector<char>(1024);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer.data(), buffer.size(), fmt, args);

	std::printf("[%s] %s\n", level, buffer.data());
	va_end(args);
}

void Log::log(const char* level, const char* func, const char* fmt, ...) {
	std::vector<char> buffer = std::vector<char>(1024);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer.data(), buffer.size(), fmt, args);

	std::printf("[%s] [%s] %s\n", level, func, buffer.data());
	va_end(args);
}

void Log::log_time(const char* level, const char* fmt, ...) {
	std::vector<char> buffer = std::vector<char>(1024);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer.data(), buffer.size(), fmt, args);

	std::printf("[%s] [%s] %s\n", Log::get_time().c_str(), level, buffer.data());
	va_end(args);
}

