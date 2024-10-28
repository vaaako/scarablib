#include "scarablib/input/mouse.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp"

void Mouse::handle_event(const SDL_Event& event) {
	// CLICK //
	if(event.type == SDL_MOUSEBUTTONDOWN) {
		SDL_MouseButtonEvent button = event.button;

		// Reset button up
		this->up = MouseBTN::NONE; // No buttons pressed
		this->down = static_cast<MouseBTN>(button.button); // Button clicked
		this->clicks = button.clicks;

		LOG_INFO("Clicked button: %d", button.button);

		// Click location
		this->click_pos.set_values(button.x, button.y);

	} else if(event.type == SDL_MOUSEBUTTONUP) {
		// Reset button down
		this->up = static_cast<MouseBTN>(event.button.button);
		this->down = MouseBTN::NONE;

		// Clear
		this->clicks = 0;
		this->click_pos.clear();
	}


	// MOTION //
	if(event.type == SDL_MOUSEMOTION) {
		SDL_MouseMotionEvent motion = event.motion;

#ifdef SCARAB_DEBUG_MOUSE
		std::printf("Mouse Motion: %i : %i\n", event.motion.x, event.motion.y);
		std::printf("Mouse dir: %i : %i\n", event.motion.xrel, event.motion.yrel);
#endif

		this->motion.set_values(motion.x, motion.y);
		this->moved_dir.set_values(event.motion.xrel, event.motion.yrel);
	}


	// SCROLL //
	if(event.type == SDL_MOUSEWHEEL) {
		this->scroll = static_cast<Scroll>(event.wheel.y);
	}
}

void Mouse::set_cursor_position(const Window& window, const uint32 x, const uint32 y) {
	int int_x = static_cast<int>(x);
	int int_y = static_cast<int>(y);
	
	if(window.out_of_bounds(int_x, int_y)) {
		LOG_ERROR("x or y args are out of window bounds");
		return;
	}

	// Change position
	SDL_WarpMouseInWindow(window.get_reference(), int_x, int_y);
	this->click_pos.set_values(x, y);
}
