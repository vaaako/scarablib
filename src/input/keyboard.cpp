#include "scarablib/input/keyboard.hpp"
#include "scarablib/proper/log.hpp"

void KeyboardHandler::handle_event(const SDL_Event& event) noexcept {
	const uint32 scancode = event.key.keysym.scancode;

	#ifdef SCARAB_DEBUG_KEYBOARD
	LOG_DEBUG("Key event: Key %d of state %d", scancode, event.key.state);
	#endif

	// If key is pressed, don't change until is released (avoid changing to down)
	if(event.key.state != 0 && this->keystate[scancode] == KeyboardHandler::State::PRESSED) {
		return;
	}

	this->keystate[scancode] = static_cast<KeyboardHandler::State>(event.key.state);
}

bool KeyboardHandler::ispressed(const Keycode key) noexcept {
	uint32 scancode = static_cast<uint32>(key);
	if (scancode >= keystate.size()) {
		return false;
	}

	// If is down, change state to "pressed"
	if(this->keystate[scancode] == KeyboardHandler::State::DOWN) {
		this->keystate[scancode] = KeyboardHandler::State::PRESSED;
		return true;
	}

	// If key is pressed, then return as false, since pressed checks one time only
	return false;
}
