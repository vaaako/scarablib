#pragma once

#include <SDL2/SDL_events.h>
#include "scarablib/typedef.hpp"
#include "scarablib/proper/vector/vec2.hpp"

enum class MouseBTN : uint8 {
	NONE = 0,
	LMB,
	MMB,
	RMB,
	SIDE1,
	SIDE2
};

enum class Scroll : uint8 {
	NONE,
	UP,
	DOWN
};

// Avoid foward declaration
class Window;

// TODO -- Vector for events so is possible to click and move in the same frame
struct MouseHandler {
	// To use handle_events in window
	friend Window;

	// Clicks made
	uint32 clicks = 0;
	// Scroll direction
	Scroll scroll = Scroll::NONE;

	// Store all actions in frame to handle later

	// Click position  (X: 0 - width / Y: 0 - height)
	vec2<uint32> click_pos;
	// Motion position (X: 0 - width / Y: 0 - height)
	vec2<uint32> motion;
	// Direction moved (Ex.: -1, 0)
	vec2<int16> moved_dir;

	// Change cursor position in window
	void set_cursor_position(const Window& window, const uint32 x, const uint32 y);

	// Check if button is clicking
	inline bool isclick(const MouseBTN button) const {
		return this->down == button;
	}

	// Check if button is up
	inline bool isup(const MouseBTN button) const {
		return this->up == button;
	}

	// TODO -- For when implement the vector of mouse events
	// Change the state of a key
	// inline void set_btnstate(const MouseBTN button, const MouseState state) {
	// 	this->btnstate[key] = state;
	// }

	private:
		// Last button up and down
		MouseBTN up = MouseBTN::NONE;
		MouseBTN down = MouseBTN::NONE;

		// std::vector<MouseState> btnstate = std::vector<MouseState>(3, MouseState::UP);

		// Handle all mouse events (used on window class only)
		void handle_event(const SDL_Event& event);
};
