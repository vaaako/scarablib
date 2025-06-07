#pragma once

#include "scarablib/typedef.hpp"

enum class Buttonstate : uint8 {
	RELEASED,
	PRESSED
};

enum class Buttoncode : uint8 {
	NONE  = 0,
	LMB,
	MMB,
	RMB,
	SIDE1,
	SIDE2
};

