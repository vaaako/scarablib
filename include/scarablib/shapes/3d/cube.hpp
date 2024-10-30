#pragma once

#include "scarablib/shapes/shape3d.hpp"

struct Cube : public Shape3D {
	Cube(const Shape3DConf& conf);

	inline VAO& get_vao() override {
		static VAO vao = VAO();
		return vao;
	}
};
