#pragma once

#include "scarablib/gfx/mesh.hpp"

struct Cube : public Mesh {
	Cube(const MeshConf& conf);

	inline const VAO& get_vao() const override {
		static const VAO vao = VAO();
		return vao;
	}
};
