#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"

// Used for configurating the window object
struct WindowConf {
	// Required values
	uint32 width = 800;
	uint32 height = 600;
	char* title = const_cast<char*>("Scarablib"); // char* in c++ goes grrr

	// Optional
	bool vsync = true;
	bool resizable = false;
	Color clear_color = Colors::WHITE; // This prevento from dividing each frame, the division only occurs on constructor (or when changing values)
	bool debug_info = false;
};
