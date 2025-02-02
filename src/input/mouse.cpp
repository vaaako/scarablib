#include "scarablib/input/mouse.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp"

void MouseHandler::handle_event(const SDL_Event& event) noexcept {
	// CLICK //
	if(event.type == SDL_MOUSEBUTTONDOWN) {
		SDL_MouseButtonEvent button = event.button;
		// Reset button up
		this->buttonstate[static_cast<uint32>(MouseHandler::Button::NONE)] = State::RELEASED; // No buttons pressed
		this->buttonstate[static_cast<uint32>(static_cast<MouseHandler::Button>(button.button))] = State::PRESSED;
		this->clicks = button.clicks;
		// Click location
		this->click_pos = { button.x, button.y };

		#ifdef SCARAB_DEBUG_MOUSE
		LOG_DEBUG("Mouse Down: Button %d at (%d, %d)", button.button, button.x, button.y);
		#endif

	} else if(event.type == SDL_MOUSEBUTTONUP) {
		// Reset button down
		this->buttonstate[static_cast<uint32>(static_cast<MouseHandler::Button>(event.button.button))] = State::RELEASED;
		this->clicks = 0;
		this->click_pos = vec3<uint32>(0); // Reset
	}


	// MOTION //
	if(event.type == SDL_MOUSEMOTION) {
		SDL_MouseMotionEvent motion = event.motion;
		this->motion = { motion.x, motion.y };
		this->moved_dir = { event.motion.xrel, event.motion.yrel };

		#ifdef SCARAB_DEBUG_MOUSE_MOTION
		std::printf("Mouse Motion: %i : %i\n", event.motion.x, event.motion.y);
		std::printf("Mouse dir: %i : %i\n", event.motion.xrel, event.motion.yrel);
		#endif
	}


	// SCROLL //
	if(event.type == SDL_MOUSEWHEEL) {
		this->scroll = (event.wheel.y > 0)
			? MouseHandler::Scroll::UP
			: MouseHandler::Scroll::DOWN;
	}
}

void MouseHandler::set_cursor_position(const Window& window, const uint32 x, const uint32 y) noexcept {
	// Change position
	SDL_WarpMouseInWindow(window.get_reference(), static_cast<int>(x), static_cast<int>(y));
}
