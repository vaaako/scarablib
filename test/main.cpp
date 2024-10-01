#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/shapes/2d/rectangle.hpp"
#include "scarablib/shapes/drawable.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/utils/file.hpp"
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
 * - Make a batch class in opengl or shapes maybe
 *
 * - Have a Scene3D like Scene2D and have a method `load_obj`, the VAO of this object will be stored in a vector, and use some key as index to get the right VAO
 * - VAO and VBO management like in Vakraft
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
	// Scene2D scene2d = Scene2D(window);


	// TODO -- make shader have a default constructor and a build constructor
	// Make build from file method
	// Make build from string method
	Shader shader = Shader(
		FileHelper::read_file("test/vertex.glsl").c_str(),
		FileHelper::read_file("test/fragment.glsl").c_str()
	);

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);

	// Update shader projection value
	// Set in order to not bind for accident
	shader.use();
	shader.set_matrix4f("projection", projection);

	// Both rectangles shares the same VAO, VBO and EBO
	Rectangle rectangle[2] = {
		Rectangle(),
		Rectangle()
	};

	Drawable drawable = Drawable();



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

		// drawable.draw_rectangle();
		drawable.draw_triangle();


		drawable.draw_circle({ 100.0f, 150.0f });
		drawable.draw_circle({ 200.0f, 150.0f }, 0.001f);

		// rectangle[0].draw(shader, { 100.0f, 100.0f });
		// rectangle[1].draw(shader, { 100.0f, 200.0f });


		window.swap_buffers();
	}

	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
