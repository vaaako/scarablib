#pragma once

#include "scarablib/gfx/model.hpp"

struct Plane : public Model {
	Plane(const ModelConf& conf) noexcept;

	inline const VAO& get_vao() const noexcept override {
		static const VAO vao = VAO();
		return vao;
	}

	// Rotates the plane front face to the camera.
	// position: Camera's position
	// axis: Axis to rotate around
	void face_position(const vec3<float>& position, const vec3<float> axis = { 0.0f, 1.0f, 0.0f }) noexcept;
};
