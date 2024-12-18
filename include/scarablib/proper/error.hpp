#pragma once

#include <string>
#include <exception>

class ScarabError : public std::exception {
	public:
		// Accepts string and C like formatting
		explicit ScarabError(const char* fmt, ...);
		~ScarabError() = default;

		const char* what() const noexcept override {
			return this->message.c_str();
		}

	private:
		std::string message;
};

