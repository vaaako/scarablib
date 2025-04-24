#pragma once

#include "scarablib/gfx/2d/sprite.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"

// Represents an ellipse
struct Circle : public Sprite {
	Circle() noexcept;

	void draw(const Camera2D& camera, const Shader& shader) noexcept override;

	// Returns the current blur value
	inline float get_blur() {
		return this->blur;
	}

	// Set a new blur value around the circle
	inline void set_blur(const float value) {
		this->blur = value;
	}

	// Circle has a different shader
	inline Shader* get_shader() const noexcept override {
		return this->shader;
	};

	private:
		float blur = 0.01f;

		// Store and get one time only (deleted inside window destructor)
		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"circle",
			Shaders::DEFAULT_VERTEX,
			Shaders::CIRCLE_FRAGMENT
		);
};
