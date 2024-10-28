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
 * but i prefer the way that is currently, maybe in the future i change it
 *
 * I can't put shaders on Shape2D because of the viewport I need to set on shader creation, unless i make it static, like circle shader
 * then the viewport would be updated on window, i could make this, maybe soon, i am too busy making 3D shapes work
 * */

// Scene object for 2D shapes
class Scene2D : public Scene {
	public:
		// Build the Scene using window object
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
		void draw_rectangle(Rectangle& rectangle, const DrawMode drawmode = DrawMode::FILLMODE);

		// Draw a triangular shape using a reference to an existing triangle object.
		// e.g., `scene2d.draw_triangle(rectangle)` or `scene2d.draw_triangle({ x, y }, { width, height }, color);`
		void draw_triangle(Triangle& triangle, const DrawMode drawmode = DrawMode::FILLMODE);

		// Draw a circular shape using a reference to an existing circle object.
		// e.g., `scene2d.draw_circle(circle)` or `scene2d.draw_circle({ x, y }, { width, height }, color);`
		void draw_circle(Circle& circle, const DrawMode drawmode = DrawMode::FILLMODE);

		// Draw any 2D shape using a reference to that object.
		// e.g., `scene2d.draw_shape(rectangle)`
		void draw_shape(Shape2D& shape, const DrawMode drawmode = DrawMode::FILLMODE);

		// Add a shape object to the scene.
		// WARNING: Shapes added to the scene are not deleted automatically, is recommended to make the shape object and then add to the scene as a pointer
		void add_to_scene(Shape2D* shape);

		// Add a vector of shape object to the scene.
		// WARNING: Shapes added to the scene are not deleted automatically, is recommended to make the shape object and then add to the scene as a pointer
		void add_to_scene(const std::vector<Shape2D*>& shapes);

		// Draw all objects in scene
		void draw_all(const DrawMode drawmode = DrawMode::FILLMODE);

		// Remove object from scene using it's index
		void remove_index(const uint32 index);

		// Get the number of objects in scene
		inline uint64 length() {
			return this->scene.size();
		}

		// Update scene viewport using window object
		inline void update_viewport(const Window& window) override {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Update scene viewport using width and height values
		void update_viewport(const uint32 width, const uint32 height) override;

	private:
		VAO* vao = new VAO();

		// Change to shared_ptr?
		std::vector<Shape2D*> scene;

		Shader* shader = new Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
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

