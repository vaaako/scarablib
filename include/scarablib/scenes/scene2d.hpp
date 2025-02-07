#pragma once

#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/gfx/2d/circle.hpp"
#include "scarablib/gfx/shape2d.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/window/window.hpp"

/**
 * I could optimize more the draw method by using "begin_draw" and "end_draw" method like raylib
 * Using these methods, I would use the same VAO bind for all shapes inside of it, which is more optimized
 * I could also make something similar to love2d, where the color is changed using `love.graphics.color`
 * and all shapes drawed after this are colorized using the color defined
 *
 * Following the same idea, i could make the same for textures, this 3 changes would be much more optimized
 * but i prefer the way that is currently, maybe in the future i change it
 *
 * I can't put shaders on Shape2D because of the viewport I need to set on shader creation, unless i make it static, like circle shader
 * then the viewport would be updated on window, i could make this, maybe soon, i am too busy making 3D shapes work
 * */

// I have to disable depth test when drawing 2D shapes
// I couldn't find out why Font gets a background when this is enabled and why 2D shapes draws below 3D shapes when this is disabled
// (i know how depth test works, but i dont know why this is happening here and not on the code pre-revamp)

// Scene object used for managing 2D objects
class Scene2D : public IScene<Shape2D> {
	public:
		// Build the Scene using the window object
		Scene2D(const Window& window) noexcept;
		~Scene2D() noexcept;

		// Disable copy and moving
		Scene2D(const Scene2D&) = delete;
		Scene2D& operator=(const Scene2D&) = delete;
		Scene2D(Scene2D&&) = delete;
		Scene2D& operator=(Scene2D&&) = delete;

		// Add a shape to the scene
		void add_to_scene(const std::string& key, Shape2D* shape) override;

		// Draw all objects added to the scene
		void draw_all() const noexcept override;

		// Update scene viewport using the window object
		inline void update_viewport(const Window& window) noexcept override {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Update scene viewport using custon width and height values
		void update_viewport(const uint32 width, const uint32 height) noexcept override;

	private:
		// TODO: Make struct to store these tree? (add a method to bind, unbind etc)
		VAO* vao = new VAO();
		VBO* vbo = new VBO();

		uint32 width;
		uint32 height;

		Shader* shader = new Shader(
			FileHelper::read_file(THIS_FILE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(THIS_FILE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
		);
};

