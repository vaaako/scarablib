#pragma once

#include "scarablib/typedef.hpp"
struct PhysicsComponent {
	vec3<float> velocity     = vec3<float>(0.0f);
	vec3<float> acceleration = vec3<float>(0.0f); // optional
	// If true, is not affected by gravity
	bool isstatic  = false;
	bool on_ground = false;
};
