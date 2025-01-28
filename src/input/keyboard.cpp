#include "scarablib/input/keyboard.hpp"
#include <cstdio>

void KeyboardHandler::handle_event(const SDL_Event& event) noexcept {
	uint32 scancode = event.key.keysym.scancode;

#ifdef SCARAB_DEBUG_KEYBOARD
	std::printf("Key event: %i - %i\n", scancode, event.key.state);
#endif

	// If key is pressed, don't change until is released (avoid changing to down)
	if(event.key.state != 0 && this->keystate[scancode] == Keystate::PRESSED) {
		return;
	}

	this->keystate[scancode] = static_cast<Keystate>(event.key.state);
}

bool KeyboardHandler::ispressed(const Keycode key) noexcept {
	uint32 scancode = static_cast<uint32>(key);

	// If is down, change state to "pressed"
	if(this->keystate.at(scancode) == Keystate::DOWN) {
		this->keystate[scancode] = Keystate::PRESSED;
		return true;
	}

	// If key is pressed, then return as false, since pressed checks one time only
	return false;
}
