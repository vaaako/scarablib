#include "scarablib/proper/result.hpp"

Result::Result(const char* message, bool success) noexcept
	: message(message), success(success) {}

Result::Result(bool success) noexcept : message(success ? "Ok!" : "Unknown Error"), success(success) {}

