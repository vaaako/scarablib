#pragma once

#include "scarablib/gfx/shape2d.hpp"
#include "scarablib/utils/file.hpp"

// Circle shape object, used to draw circle
struct Circle : public Shape2D {
	// To update viewport (not making a static variable because only Shape2D needs to access it)
	friend class Scene2D;

	// Initialize current shape using the Shape2DConf struct
	Circle(const Shape2DConf& conf);

	// Draw the circle using the circle shader.
	// No shader is needed here, since Circle has a different shader stored in the struct. This method is a virtual method from Shape2D.
	void draw(const Shader& shader) override;

	// Get the current blur value
	inline float get_blur() {
		return this->blur;
	}

	// Set a blur value around the circle
	inline void set_blur(const float blur) {
		this->blur = blur;
	}

	private:
		float blur = 0.01f;

		// Make static so Scene2D can update viewport
		static inline Shader& get_shader() {
			static Shader shader_circle = Shader(
				FileHelper::read_file(SOURCE_DIR + "/../../opengl/shaders/2d/vertex.glsl").c_str(),
				FileHelper::read_file(SOURCE_DIR + "/../../opengl/shaders/2d/circle_fragment.glsl").c_str()
			);

			return shader_circle;
		}

};
