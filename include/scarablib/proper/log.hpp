#pragma once

#include <string>

// Namespace with methods used for logging
namespace Log {
	// Returns date formatteed as YYYY-MM-DD
	std::string get_date() noexcept;
	// Returns time formatted as HH:MM:SS
	std::string get_time() noexcept;
	// Returns date and time formatted as YYYY-MM-DD HH:MM:SS
	std::string get_date_and_time() noexcept;

	// Implementation
	void log_impl(const char* level, const char* func, bool include_time, const char* fmt, ...) noexcept;
};



// NOTE -- I am only using macros here because to use __func__

// ##__VA_ARGS__: optional
//   __VA_ARGS__: required

// Simple log, just print a message
#define LOG(fmt, ...)              Log::log_impl(nullptr, nullptr, false, fmt, ##__VA_ARGS__)

// Log with a custom level
#define LOG_FLAG(level, fmt, ...)  Log::log_impl(level, nullptr, false, fmt, ##__VA_ARGS__)

// Log with predefined levels (no function name)
#define LOG_INFO(fmt, ...)         Log::log_impl("INFO",    nullptr, false, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...)      Log::log_impl("WARNING", nullptr, false, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)        Log::log_impl("ERROR",   nullptr, false, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)        Log::log_impl("DEBUG",   nullptr, false, fmt, ##__VA_ARGS__)

// Log with predefined levels and time
#define LOG_INFO_TIME(fmt, ...)    Log::log_impl("INFO",    nullptr, true, fmt ##__VA_ARGS__)
#define LOG_WARNING_TIME(fmt, ...) Log::log_impl("WARNING", nullptr, true, fmt ##__VA_ARGS__)
#define LOG_ERROR_TIME(fmt, ...)   Log::log_impl("ERROR",   nullptr, true, fmt ##__VA_ARGS__)

// Log with predefined levels and function name
#define LOG_INFO_FN(fmt, ...)      Log::log_impl("INFO",    __func__, false, fmt, ##__VA_ARGS__)
#define LOG_WARNING_FN(fmt, ...)   Log::log_impl("WARNING", __func__, false, fmt, ##__VA_ARGS__)
#define LOG_ERROR_FN(fmt, ...)     Log::log_impl("ERROR",   __func__, false, fmt, ##__VA_ARGS__)

