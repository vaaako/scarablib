#pragma once

struct Result {
	const char* message = nullptr;
	bool success;

	Result() = default;
	Result(const char* message, bool success) noexcept;
	Result(bool success) noexcept;

	// Checks directly to a boolean
	bool operator==(const bool value) const noexcept {
		return this->success == value;
	}
};
