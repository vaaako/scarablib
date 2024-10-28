#pragma once

#include "scarablib/shapes/3d/mesh.hpp"

struct Cube : public Mesh {
	Cube();

	private:
		inline VAO& get_vao() override {
			static VAO vao = VAO();
			return vao;
		}
};
