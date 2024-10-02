#pragma once

#include "scarablib/opengl/vao.hpp"
#include "scarablib/scenes/scene.hpp"
#include "scarablib/shapes/2d/circle.hpp"
#include "scarablib/shapes/2d/rectangle.hpp"
#include "scarablib/shapes/2d/triangle.hpp"
#include "scarablib/shapes/shape2d.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/window/window.hpp"

/**
 * I could optimize more the draw method by using "begin_draw" and "end_draw" method like raylib
 * Using these methods, I would use the same VAO bind for all shapes inside of it, which is more optimized
 *
 * I could also make something similar to love2d, where the color is changed using `love.graphics.color`
 * and all shapes drawed after this are colorized using the color defined
 * */

class Scene2D : public Scene {
	public:
		Scene2D(const Window& window);
		~Scene2D();

		// Draw a rectangular shape using a reference.
		// e.g., `scene2d.draw_rectangle(rectangle)` or `scene2d.draw_rectangle({ x, y }, { width, height }, color);`
		void draw_rectangle(const Rectangle& rectangle) const;

		// Draw a triangular shape using a reference.
		// e.g., `scene2d.draw_triangle(rectangle)` or `scene2d.draw_triangle({ x, y }, { width, height }, color);`
		void draw_triangle(const Triangle& triangle) const;

		// Draw a circular shape using a reference.
		// e.g., `scene2d.draw_circle(circle)` or `scene2d.draw_circle({ x, y }, { width, height }, color);`
		void draw_circle(const Circle& circle) const;

		// Draw any 2D shape using a reference.
		// e.g., `scene2d.draw_shape(rectangle)`
		void draw_shape(const Shape2D& shape);

		// Update scene viewport using window object
		inline void update_viewport(const Window& window) {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Update scene viewport using width and height values
		void update_viewport(const uint32 width, const uint32 height);

	private:
		VAO* vao = new VAO();

		// Storing all shaders
		std::vector<Shader*> shaders = {
			new Shader(
				FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
				FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
			),

			new Shader(
				FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
				FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/circle_fragment.glsl").c_str()
			)
		};

		// Better acess
		Shader& shader = *shaders.at(0);
		Shader& shader_circle = *shaders.at(1);
};

