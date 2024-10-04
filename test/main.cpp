#include "scarablib/opengl/ebo.hpp"
#include "scarablib/scenes/scene2d.hpp"
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
 * - Prevent from making a new object each frame
 * - Share texture when is the case
 * - Shader uniforms changes only when needed
 * - Model matrix make calculation only when needed
 *
 * - Use `scene.draw_shapes` for batching
 * - batch will not be possible manually, probably needing to add methods like `scene.add_to_pool()` and `scene.draw_batch()`
 *
 * - Have a Scene3D like Scene2D and have a method `load_obj`, the VAO of this object will be stored in a vector, and use some key as index to get the right VAO
 * - VAO and VBO management like in Vakraft
 *
 * Start 3D implementation only when draw methods are solid
 * */


// TODO -- memory leak somewhere

int main() {
	Window window = Window({
		.width = 800,
		.height = 600,
		.clear_color = Colors::MOSS,
		.debug_info = true
	});

	// Texture texture = Texture("test/assets/images/hideri.jpg");
	Scene2D scene2d = Scene2D(window);

	Rectangle rectangle = {
		vec2 { 100.0f, 100.0f },
		vec2 { 100.0f },
		Colors::CHIROYELLOW
	};

	Triangle triangle = {
		vec2 { 150.0f, 75.0f },
		vec2 { 70.0f, 100.0f },
		Colors::MAGENTA
	};

	Circle circle = {
		vec2 { 250.0f, 175.0f },
		vec2 { 100.0f, 50.0f },
		Colors::HOTPINK
	};


	// Play music starting with a fade in
	// music.play_fadein(5000);
	while(window.is_open()) {
		window.clear();
		window.process_events();

		if(window.keyboard()->ispressed(Keycode::ESCAPE) || window.on_event(Event::QUIT)) {
			window.close();
		}

		if (window.keyboard()->ispressed(Keycode::SPACE)) {
			window.set_clear_color(Colors::MOSS);
		}

		scene2d.draw_rectangle(rectangle);
		scene2d.draw_triangle(triangle);

		scene2d.draw_circle(circle);

		// scene2d.draw_circle();

		window.swap_buffers();
	}

	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
