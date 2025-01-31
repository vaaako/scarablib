#pragma once

#include "scarablib/gfx/model.hpp"

struct Plane : public Model {
	Plane(const ModelConf& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;

	// Rotates the plane front face to the camera.
	// position: Camera's position
	// axis: Axis to rotate around
	void face_position(const vec3<float>& position, const vec3<float> axis = { 0.0f, 1.0f, 0.0f }) noexcept;
};
