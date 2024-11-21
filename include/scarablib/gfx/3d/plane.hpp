#pragma once

#include "scarablib/gfx/mesh.hpp"

struct Plane : public Mesh {
	Plane(const MeshConf& conf);

	inline const VAO& get_vao() const override {
		static const VAO vao = VAO();
		return vao;
	}

	void face_position(const vec3<float>& position, const vec3<float> axis = { 0.0f, 1.0f, 0.0f });
};
