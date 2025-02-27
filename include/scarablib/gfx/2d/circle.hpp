#pragma once

#include "scarablib/gfx/2d/sprite.hpp"
#include "scarablib/utils/file.hpp"

// Class for rectangle sprite
struct Circle : public Sprite {
	// WARNING: Do not use this constructor, use SpriteFactory
	Circle(const std::vector<Vertex>& vertices) noexcept;

	void draw(const Camera2D& camera, const Shader& shader) noexcept override;

	// Returns the current blur value
	inline float get_blur() {
		return this->blur;
	}

	// Set a new blur value around the circle
	inline void set_blur(const float blur) {
		this->blur = blur;
	}

	// Circle has a different shader
	inline Shader* get_shader() const noexcept override {
		return this->shader;
	};

	private:
		float blur = 0.01f;

		Shader* shader = new Shader(
			FileHelper::read_file("resources/shaders/vertex.glsl").c_str(),
			FileHelper::read_file("resources/shaders/2d/circle_fs.glsl").c_str()
		);
};
