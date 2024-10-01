#pragma once

// #include "scarablib/types/font.hpp"
#include "scarablib/scenes/scene.hpp"
#include "scarablib/shapes/shape2d.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/window/window.hpp"

class Scene2D : public Scene {
	public:
		Scene2D(const Window& window);

		// Draw a single shape
		inline void draw_shape(const Shape2D& shape, const DrawMode drawmode = DrawMode::FILLMODE, const DrawType drawtype = DrawType::TRIANGLES) {
			shape.draw(
				this->shader,
				drawmode, drawtype
			);
		}

		// inline void draw_font(Font& font) const {
		// 	font.draw2d(*this->shader_texture, DrawMode::FILL, GL_TRIANGLE_FAN);
		// }

		// Update scene viewport using window object
		inline void update_viewport(const Window& window) {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Update scene viewport using width and height values
		void update_viewport(const uint32 width, const uint32 height);

		// Draw multiple shapes. You can draw how many shapes you want
		//
		// Ex.: `draw_shapes(shape1, shape2, shape3)`
		template <typename... Shapes>
		void draw_shapes(const Shapes&... meshes) const {
			std::vector<const Shape2D*> buffer  = { &meshes... };

			// Draw each from vector
			for(const Shape2D* shape : buffer) {
				shape->draw(
					this->shader,
					DrawMode::FILLMODE
				);
			}
		}

		// Draw multiple shapes with a common DrawMode. You can draw how many shapes you want
		//
		// Ex.: `draw_shapes(DrawMode::OUTLINEMODE, shape1, shape2, shape3)`
		template <typename... Shapes>
		void draw_shapes(const DrawMode drawmode, const Shapes&... meshes) const {
			std::vector<const Shape2D*> buffer  = { &meshes... };

			// Draw each from vector
			for(const Shape2D* shape : buffer) {
				shape->draw(
					this->shader,
					drawmode
				);
			}
		}

	private:
		// Shader need to be in header to find files in include/

		Shader shader = Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
		);
};

