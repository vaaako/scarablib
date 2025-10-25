#pragma once

#include "scarablib/geometry/sprite.hpp"

// Represents an ellipse
struct Circle : public Sprite {
	// Amount of blur on circle's border. Clamped: 0.001 to 1.0
	float blur = 0.01f;

	Circle() noexcept;

	// This method does not draw the model to the screen, as it does not bind the VAO and Shader
	void draw_logic(const Camera& camera) noexcept override;
};
