#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/shapes/2d/rectangle.hpp"
#include "scarablib/shapes/2d/triangle.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp"
#include "scarablib/input/keycode.hpp"
#include <cstdio>

/* Optimization Features:
 * - Batch Rendering
 * - Object Pooling for VAOs and Textures 
 *
 * - Unify shaders
 * - Better Shape storage
 *   + I am not happy with how I am storing each shape vertices and indices
 * - Prevent from making a new object each frame
 * - Share VAO
 * - Share texture when is the case
 * - Shader uniforms changes only when needed
 * - Model matrix make calculation only when needed
 *
 *
 * Start 3D implementation only when draw methods are solid
 * */


int main() {
	Window window = Window({
		.width = 800,
		.height = 600,
		.clear_color = 0xca1773,
		.debug_info = true
	});

	Texture texture = Texture("test/hideri.jpg");
	Scene2D scene2d = Scene2D(window);

	Triangle triangle = Triangle();

	// Play music starting with a fade in
	// music.play_fadein(5000);
	while(window.is_open()) {
		// TODO -- Save clean color to avoid division each frame
		window.clear();
		window.process_events();

		if(window.keyboard()->ispressed(Keycode::ESCAPE) || window.on_event(Event::QUIT)) {
			window.close();
		}

		if (window.keyboard()->ispressed(Keycode::SPACE)) {
			window.set_clear_color(Colors::MOSS);
		}

		scene2d.draw_shape(
			triangle
			.set_texture(&texture)
			// .set_color(Colors::MOSS)
			.set_size(200.0f)
			.set_position({ 400.0f, 300.0f })
		);

		window.swap_buffers();
	}

	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
