#include "scarablib/opengl/ebo.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/types/font.hpp"
#include "scarablib/window/window.hpp"
#include "scarablib/input/keycode.hpp"
#include <cstdio>
#include <iomanip>
#include <sstream>

/* Optimization Features:
 * - Batch Rendering
 * - Object Pooling for VAOs and Textures 
 *
 * - Model matrix make calculation only when needed
 *
 * - Have a Scene3D like Scene2D and have a method `load_obj`, the VAO of this object will be stored in a vector, and use some key as index to get the right VAO
 * - VAO and VBO manager
 *
 * */

// - review scene.hpp
// - font
// - inline shape class?

// TODO -- memory leak somewhere
// - Window not unloading correctly?

int main() {
	// Make window object
	Window window = Window({
		.width = 800,
		.height = 600,
		.clear_color = Colors::ORANGE,
		.debug_info = true
	});

	// Load assets
	Texture tex1 = Texture("test/assets/images/hideri.jpg");
	Texture tex2 = Texture("test/assets/images/kuromi.png");
	Font msgothic = Font("test/assets/fonts/msgothic.ttf", "FPS: 0", 24);

	// Make shapes
	Rectangle rectangle = {
		vec2 { 500.0f, 150.0f },
		vec2 { 200.0f },
	};
	// Bind texture
	rectangle.set_texture(&tex2);

	Triangle triangle {
		vec2 { 150.0f, 75.0f },
		vec2 { 70.0f, 100.0f },
		Colors::MAGENTA
	};

	Circle circle = {
		vec2 { 250.0f, 175.0f },
		vec2 { 100.0f, 50.0f },
		Colors::HOTPINK
	};

	// Make scene and add shapes
	Scene2D scene2d = Scene2D(window);

	// Add shapes to scene
	scene2d.add_to_scene({
		&rectangle,
		&triangle,
		&circle
	});


	while(window.is_open()) {
		// Clear screen
		window.clear();
		// Get events
		window.process_events();

		// Handle input
		if(window.keyboard()->ispressed(Keycode::ESCAPE) || window.on_event(Event::QUIT)) {
			window.close();
		}

		if(window.keyboard()->ispressed(Keycode::Q)) {
			rectangle.remove_texture();
		}

		// Draw font
		std::stringstream stream; stream << std::setprecision(2) << window.fps();
		msgothic.set_text("FPS:" + stream.str());
		scene2d.draw_rectangle(msgothic);

		// Draw rectangle
		rectangle.set_angle((float)window.time() / 10);

		// Draw shape
		scene2d.draw_all();

		// Swap buffers
		window.swap_buffers();
	}

	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
