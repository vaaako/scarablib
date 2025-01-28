#pragma once

#include "scarablib/gfx/model.hpp"
#include "scarablib/opengl/vao.hpp"

struct Cube : public Model {
	Cube(const ModelConf& conf) noexcept;

	inline const VAO& get_vao() const noexcept override {
		static const VAO vao = VAO();
		return vao;
	}
};
