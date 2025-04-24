#pragma once

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>

#include "scarablib/input/keycode.hpp"
#include "scarablib/typedef.hpp"

// Avoid foward declaration
class Window;

class KeyboardHandler {
	// To use handle_event in window
	friend Window;

	public:
		// The state of a key
		enum class State : uint8 {
			RELEASED,
			DOWN,
			PRESSED // Called it one time only
		};

		KeyboardHandler() noexcept = default;

		// Delete move and copy
		KeyboardHandler(const KeyboardHandler&) noexcept = delete;
		KeyboardHandler& operator=(const KeyboardHandler&) noexcept = delete;
		KeyboardHandler(KeyboardHandler&&) noexcept = delete;
		KeyboardHandler& operator=(KeyboardHandler&&) noexcept = delete;

		// Check if a key was pressed
		bool ispressed(const Keycode key) noexcept;

		// Check if a key is down
		inline bool isdown(const Keycode key) const noexcept {
			return (this->keystate.at(static_cast<uint32>(key)) == KeyboardHandler::State::DOWN);
		}

		// Check if a key is released
		inline bool isup(const Keycode key) const noexcept {
			return (this->keystate.at(static_cast<uint32>(key)) == KeyboardHandler::State::RELEASED);
		}

		// Returns the state of a key
		inline KeyboardHandler::State get_keystate(const Keycode key) const noexcept {
			return this->keystate[static_cast<uint32>(key)];
		}

		// Changes the state of a key
		inline void set_keystate(const Keycode key, const KeyboardHandler::State state) noexcept {
			this->keystate[static_cast<uint32>(key)] = state;
		}


	private:
		// Init vector with the size of SDL scancodes with the value 0 (RELEASED)
		std::vector<KeyboardHandler::State> keystate
			= std::vector<KeyboardHandler::State>(
					(uint32)Keycode::NUM_SCANCODES, KeyboardHandler::State::RELEASED);

		// Handle all keyboard events (used on window class only)
		void handle_event(const SDL_Event& event) noexcept;
};
