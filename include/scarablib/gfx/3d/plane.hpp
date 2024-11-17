#pragma once

#include "scarablib/gfx/mesh.hpp"

struct Plane : public Mesh {
	Plane(const MeshConf& conf);

	inline const VAO& get_vao() const override {
		static const VAO vao = VAO();
		return vao;
	}
};
