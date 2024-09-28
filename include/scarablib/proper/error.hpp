#pragma once

#include <exception>

class ScarabError : public std::exception {
	public:
		// Accepts string and C like formatting
		ScarabError(const char* fmt, ...);
		~ScarabError();

		// ScarabError(std::function<void()> callback, const char* fmt, ...) {
		// 	callback();
		// }

		const char* what() const noexcept override {
			return this->message;
		}

	private:
		char* message;
};

