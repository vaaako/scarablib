#pragma once

#include "scarablib/shapes/mesh.hpp"

struct Cube : public Mesh {
	Cube(const MeshConf& conf);

	inline const VAO& get_vao() override {
		static const VAO vao = VAO();
		return vao;
	}
};
