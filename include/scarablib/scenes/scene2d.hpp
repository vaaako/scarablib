#pragma once

#include "scarablib/opengl/vao.hpp"
#include "scarablib/scenes/scene.hpp"
#include "scarablib/shapes/2d/circle.hpp"
#include "scarablib/shapes/2d/rectangle.hpp"
#include "scarablib/shapes/2d/triangle.hpp"
#include "scarablib/shapes/shape2d.hpp"
#include "scarablib/types/font.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/window/window.hpp"

/**
 * I could optimize more the draw method by using "begin_draw" and "end_draw" method like raylib
 * Using these methods, I would use the same VAO bind for all shapes inside of it, which is more optimized
 *
 * I could also make something similar to love2d, where the color is changed using `love.graphics.color`
 * and all shapes drawed after this are colorized using the color defined
 *
 * Following the same idea, i could make the same for textures, this 3 changes would be much more optimized
 * but i prefer the way that is currently, maybe sometime i change
 *
 * I can't put shaders on Scene2D because of the viewport I need to set on shader creation
 * */

class Scene2D : public Scene {
	public:
		Scene2D(const Window& window);
		~Scene2D();

		// Disable copy and moving
		Scene2D(const Scene2D&) = delete;
		Scene2D& operator=(const Scene2D&) = delete;
		Scene2D(Scene2D&&) = delete;
		Scene2D& operator=(Scene2D&&) = delete;

		// Draw a font using a reference to an existing font object.
		// This is the same as using `draw_rectangle`, just a different name
		void draw_font(Font& font);

		// Draw a rectangular shape using a reference to an existing rectangle object.
		// e.g., `scene2d.draw_rectangle(rectangle)` or `scene2d.draw_rectangle({ x, y }, { width, height }, color);`
		void draw_rectangle(Rectangle& rectangle);

		// Draw a triangular shape using a reference to an existing triangle object.
		// e.g., `scene2d.draw_triangle(rectangle)` or `scene2d.draw_triangle({ x, y }, { width, height }, color);`
		void draw_triangle(Triangle& triangle);

		// Draw a circular shape using a reference to an existing circle object.
		// e.g., `scene2d.draw_circle(circle)` or `scene2d.draw_circle({ x, y }, { width, height }, color);`
		void draw_circle(Circle& circle);

		// Draw any 2D shape using a reference to that object.
		// e.g., `scene2d.draw_shape(rectangle)`
		void draw_shape(Shape2D& shape);

		// Add a shape object to the scene.
		// WARNING: Shapes added to the scene are not deleted automatically, is recommended to make the shape object and then add to the scene as a pointer
		void add_to_scene(Shape2D* shape);

		// Add a vector of shape object to the scene.
		// WARNING: Shapes added to the scene are not deleted automatically, is recommended to make the shape object and then add to the scene as a pointer
		void add_to_scene(const std::vector<Shape2D*> shapes);

		// Draw all objects in scene
		void draw_all();

		// Remove object from scene using it's index
		void remove_index(const uint32 index);

		// Get the number of objects in scene
		inline uint64 length() {
			return this->scene.size();
		}

		// Update scene viewport using window object
		inline void update_viewport(const Window& window) {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Update scene viewport using width and height values
		void update_viewport(const uint32 width, const uint32 height);

	private:
		VAO* vao = new VAO();
		// Change to shared_ptr?
		std::vector<Shape2D*> scene;

		Shader* shader = new Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
		);

		Shader* shader_circle = new Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/circle_fragment.glsl").c_str()
		);

		inline void begin_draw() {
			this->shader->use();
			this->vao->bind();
		}

		inline void end_draw() {
			this->vao->unbind();
			this->shader->unbind();
		}
};

