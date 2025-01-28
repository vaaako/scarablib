#include "scarablib/proper/log.hpp"

#include <cstdarg>
#include <ctime>
#include <mutex>
#include <sstream>
#include <vector>

std::string Log::get_date() noexcept {
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	std::ostringstream oss;
	oss << (localTime->tm_year + 1900) << '-'
		<< (localTime->tm_mon + 1) << '-'
		<< localTime->tm_mday << ' ';
	return oss.str();
}

std::string Log::get_time() noexcept {
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	std::ostringstream oss;
	oss << localTime->tm_hour << ':'
		<< localTime->tm_min << ':'
		<< localTime->tm_sec;
	return oss.str();
}

std::string Log::get_date_and_time() noexcept {
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

//
// void Log::log(const char* level, const char* fmt, ...) {
// 	va_list args;
// 	va_start(args, fmt);
// 	log_impl(fmt, level, nullptr, args);
// 	va_end(args);
// }

// void Log::log_time(const char* level, const char* fmt, ...) {
// 	va_list args;
// 	va_start(args, fmt);
// 	log_impl(fmt, level, nullptr, args, true);
// 	va_end(args);
// }

void Log::log_impl(const char* level, const char* func, bool include_time, const char* fmt, ...) noexcept {
	va_list args;
	va_start(args, fmt);

	std::vector<char> buffer = std::vector<char>(1024);
	size_t needed = vsnprintf(buffer.data(), buffer.size(), fmt, args);

	// Resize buffer if the formatted string is too large
	if (needed >= buffer.size()) {
		buffer.resize(needed + 1);
		vsnprintf(buffer.data(), buffer.size(), fmt, args);
	}

	if(include_time) {
		std::printf("[%s] [%s] %s\n", get_time().c_str(), level ? level : "", buffer.data());
	} else if(level) {
		std::printf("[%s] %s\n", level, buffer.data());
	} else if(func) {
		std::printf("[%s] [%s] %s\n", level ? level : "", func, buffer.data());
	} else {
		std::printf("%s\n", buffer.data());
	}

	va_end(args);
}
