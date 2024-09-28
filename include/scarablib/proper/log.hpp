#pragma once

#include <string>

// Namespace with methods used for logging
namespace Log {
	std::string get_date();
	std::string get_time();
	std::string get_date_and_time();

	// Simplest log, just print a message
	void log(const char* fmt, ...);

	// Log with flag
	void log(const char* level, const char* fmt, ...);

	// Log with a flag, showing the method it came from
	void log(const char* level, const char* func, const char* fmt, ...);

	// Log with time
	void log_time(const char* level, const char* fmt, ...);
};



// NOTE -- I am only using macros here because to use __func__, so making all macros helps standardize them

// ##__VA_ARGS__: optional
//   __VA_ARGS__: required

#define LOG(fmt, ...) Log::log(fmt, ##__VA_ARGS__)
#define LOG_FLAG(level, fmt, ...) Log::log(level, fmt, ##__VA_ARGS__)

// No function name
#define LOG_INFO(fmt, ...) Log::log("INFO", fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) Log::log("WARNING", fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Log::log("ERROR", fmt, ##__VA_ARGS__)

// Time
#define LOG_INFO_TIME(fmt, ...) Log::lo_time("INFO", fmt, ##__VA_ARGS__)
#define LOG_WARNING_TIME(fmt, ...) Log::log_time("WARNING", fmt, ##__VA_ARGS__)
#define LOG_ERROR_TIME(fmt, ...) Log::log_time("ERROR", fmt, ##__VA_ARGS__)

// Show function
#define LOG_INFO_FN(fmt, ...) Log::log("INFO", __func__, fmt, ##__VA_ARGS__)
#define LOG_WARNING_FN(fmt, ...) Log::log("WARNING", __func__, fmt, ##__VA_ARGS__)
#define LOG_ERROR_FN(fmt, ...) Log::log("ERROR", __func__, fmt, ##__VA_ARGS__)

