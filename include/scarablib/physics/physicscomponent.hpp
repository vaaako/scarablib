#pragma once

#include "scarablib/typedef.hpp"

struct PhysicsComponent {
	// The current velocity of the object
	vec3<float> velocity     = vec3<float>(0.0f);
	// optional
	vec3<float> acceleration = vec3<float>(0.0f); // optional
	// Mass of the object
	float mass = 1.0f;
	// If true, is not affected by gravity
	bool isstatic  = false;
	// Just used to check if the object is on the ground
	bool on_ground = false;

	// void apply_gravity(const Mesh& mesh, const float dt) {
	// }
};
