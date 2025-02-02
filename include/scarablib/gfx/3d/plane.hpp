#pragma once

#include "scarablib/gfx/model.hpp"

// This is a class to make a plane model
// WARNING: Do not use this class directly, use ModelFactory::create_cube(const ModelConf& conf)
struct Plane : public Model {
	// WARNING: Do not use this class directly, use ModelFactory::create_cube(const ModelConf& conf)
	Plane(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;

	// Rotates the plane front face to the camera.
	// position: Camera's position
	// axis: Axis to rotate around
	void face_position(const vec3<float>& position, const vec3<float> axis = { 0.0f, 1.0f, 0.0f }) noexcept;
};
