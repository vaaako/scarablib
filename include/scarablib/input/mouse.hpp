#pragma once

#include <SDL2/SDL_events.h>
#include <vector>
#include <SDL2/SDL_mouse.h>
#include "scarablib/typedef.hpp"

// Avoid foward declaration
class Window;

class MouseHandler {
	// To use handle_events in window
	friend Window;

	public:
		// Mouse buttons
		enum class Button : uint8 {
			NONE  = 0,
			LMB   = 1,
			MMB   = 2,
			RMB   = 3,
			SIDE1 = 4,
			SIDE2 = 5
		};

		// State of a button
		enum class State : uint8 {
			RELEASED,
			PRESSED
		};

		// Scroll direction
		enum class Scroll : uint8 {
			NONE,
			UP,
			DOWN
		};

		MouseHandler() noexcept = default;

		// Delete move and copy
		MouseHandler(const MouseHandler&) noexcept = delete;
		MouseHandler& operator=(const MouseHandler&) noexcept = delete;
		MouseHandler(MouseHandler&&) noexcept = delete;
		MouseHandler& operator=(MouseHandler&&) noexcept = delete;


		// Returns mouse click position
		inline vec2<uint32> get_click_pos() const noexcept {
			return this->click_pos;
		}

		// Returns mouse motion/position
		inline vec2<uint32> get_motion() const noexcept {
			return this->motion;
		}

		// Returns mouse moved direction (e.g. -1, 1 = Left Down)
		inline vec2<int16> get_moved_dir() const noexcept {
			return this->moved_dir;
		}

		// Returns a button last event
		inline MouseHandler::State get_last_event(const MouseHandler::Button button) const noexcept {
			return this->buttonstate[static_cast<uint32>(button)];
		}

		// Returns mouse movent since last call
		inline vec2<int32> relative_move() const noexcept {
			vec2<int32> mov;
			SDL_GetRelativeMouseState(&mov.x, &mov.y);
			return mov;
		}

		// Checks if a button is clicking
		inline bool isclick(const MouseHandler::Button button) const noexcept {
			return this->buttonstate[static_cast<uint32>(button)] == MouseHandler::State::PRESSED;
		}

		// Checks if a button is released
		inline bool isup(const MouseHandler::Button button) const noexcept {
			return this->buttonstate[static_cast<uint32>(button)] == MouseHandler::State::RELEASED;
		}

		// Checks the state of a button
		inline MouseHandler::State get_keystate(const MouseHandler::Button button) const {
			return this->buttonstate[static_cast<uint32>(button)];
		}

		// Changes the state of a button
		inline void set_keystate(const MouseHandler::Button button, const MouseHandler::State state) noexcept {
			this->buttonstate[static_cast<uint32>(button)] = state;
		}

		// Changes cursor position in window
		void set_cursor_position(const Window& window, const uint32 x, const uint32 y) noexcept;

	private:
		// Last button up and down
		MouseHandler::Button up = MouseHandler::Button::NONE;
		MouseHandler::Button down = MouseHandler::Button::NONE;

		// Store the state of all mouse buttons (LMB, MMB, RMB, SIDE1, SIDE2)
		std::vector<MouseHandler::State> buttonstate = std::vector<MouseHandler::State>(6, MouseHandler::State::RELEASED);

		// Clicks made
		uint32 clicks = 0;
		// Scroll direction
		Scroll scroll = Scroll::NONE;

		// Store all actions in frame to handle later

		// Click position  (X: 0 -> width / Y: 0 -> height)
		vec2<uint32> click_pos;
		// Motion position (X: 0 -> width / Y: 0 -> height)
		vec2<uint32> motion;
		// Direction moved (Ex.: -1, 0 = Left)
		vec2<int16> moved_dir; // xrel yrel

		// Handle all mouse events (used on window class only)
		void handle_event(const SDL_Event& event) noexcept;
};

