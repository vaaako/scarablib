#pragma once

#include "scarablib/gfx/model.hpp"
#include "scarablib/opengl/vao.hpp"

struct Cube : public Model {
	Cube(const ModelConf& conf);

	inline const VAO& get_vao() const override {
		static const VAO vao = VAO();
		return vao;
	}
};
