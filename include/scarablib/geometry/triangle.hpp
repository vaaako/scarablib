#pragma once

#include "scarablib/typedef.hpp"

// Represents a triangle of a mesh
struct MeshTriangle {
	vec3<float> v0;
	vec3<float> v1;
	vec3<float> v2;
	vec3<float> normal;
};
