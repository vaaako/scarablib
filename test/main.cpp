#include "scarablib/opengl/ebo.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/scenes/scene3d.hpp"
#include "scarablib/shapes/3d/cube.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/types/font.hpp"
#include "scarablib/window/window.hpp"
#include "scarablib/input/keycode.hpp"
#include <cstdio>
#include <iomanip>
#include <sstream>

bool can_move = false;

void camera_movement(Window& window, Camera& camera, KeyboardHandler& keyboard) {
	if(keyboard.isdown(Keycode::W)) {
		camera.move_front();
	} else if (keyboard.isdown(Keycode::S)) {
		camera.move_back();
	}

	if(keyboard.isdown(Keycode::A)) {
		camera.move_left();
	} else if (keyboard.isdown(Keycode::D)) {
		camera.move_right();
	}

	if(keyboard.isdown(Keycode::SPACE)) {
		camera.fly_up();
	} else if(keyboard.isdown(Keycode::LSHIFT)) {
		camera.fly_down();
	}

	// Press TAB to not rotate anymore
	if(can_move && keyboard.ispressed(Keycode::ESCAPE)) {
		can_move = false;
		window.hide_cursor(false);
	}
}

void rotate_camera(Window& window, Camera& camera, MouseHandler& mouse) {
	// ROTATION //
	// Only rotate when click on screen
	if(mouse.isclick(MouseBTN::LMB) && !can_move) {
		LOG_INFO("Clicked");
		can_move = true;
		mouse.set_cursor_position(window, window.get_width() / 2, window.get_height() / 2); // Prevent cursor from exiting the screen
		window.hide_cursor(true);
		return; // Prevent camera from look where the cursor clicked
	}

	// Rotate camera
	if(can_move) {
		camera.rotate(mouse);
		mouse.set_cursor_position(window, window.get_width() / 2, window.get_height() / 2); // Prevent cursor from exiting the screen
	}
}


// TODO -- memory leak somewhere
// - Window not unloading correctly?
// - stbi maybe?
int main() {
	// TODO:
	// - Keyboard and Mouse public instead of a getter
	// - Mouse handle multiple inputs like keyboard
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = const_cast<char*>("Something idk"),
		.clear_color = Colors::ORANGE,
		.vsync = true,
		.debug_info = true
	});

	// Load assets
	Texture tex1 = Texture("test/assets/images/kuromi.png");
	Texture tex2 = Texture("test/assets/images/purin.png");

	// TODO:
	// - Remove background (GL_DEPTH_TEST is causing this or something else)
	Font msgothic = Font("test/assets/fonts/msgothic.ttf", 24);

	Camera camera = Camera(window, 75.0f);

	// TODO:
	// - Instead of storing VAO object, store ID only
	Scene2D scene2d = Scene2D(window);
	Scene3D scene3d = Scene3D(window, camera);

	Cube cube = Cube({
		.position = 0.0f,
	});
	cube.set_texture(&tex1);

	Cube cube2 = Cube({
		.position = { 0.0f, 0.0f, -5.0f },
		.size = 0.5f
	});
	cube2.set_texture(&tex2);

	Circle circle = Circle({
		.position = { 400.0f },
		.size = { 50.0f }
	});

	Rectangle rectangle = Rectangle({
		.position = { 400.0f, 400.0f },
		.size = { 50.0f, 50.0f },
	});
	// rectangle.set_texture(&tex1);


	float rotation = 0.0f;
	float rotation_speed = 1.0f;
	while(window.is_open()) {
		// Clear screen
		window.clear();
		// Get events
		window.process_events();

		// Handle input
		if((!can_move && window.keyboard().ispressed(Keycode::ESCAPE)) || window.on_event(Event::QUIT)) {
			window.close();
		}

		// Handle camera keyboard inputs
		camera_movement(window, camera, window.keyboard());
		// Handle camera mouse inputs
		rotate_camera(window, camera, window.mouse());

		// WARNING -- When drawing 3D and 2D shapes together, draw 3D shapes first

		// Draw 3D shapes
		scene3d.draw_mesh(cube);

		// Set position to rotate around first cube
		scene3d.draw_mesh(
			cube2.set_position(vecutil::orbitate_y(cube.get_position(), rotation, 2.0f))
		);

		// Rotate
		rotation += rotation_speed;
		if(rotation >= 360.0f) {
			rotation = 0.0f; // Wrap around to keep the angle within 0-360 degrees
		}

		// Draw 2D shapes
		// Draw texts
		// Format FPS, ignore
		std::stringstream stream; stream << std::setprecision(2) << window.fps();
		scene2d.draw_shape(msgothic.set_text("FPS: " + stream.str()).set_position(0.0f));
		scene2d.draw_shape(msgothic.set_text("TESTING").set_position({ 0.0f, 24.0f }));

		scene2d.draw_shape(rectangle);

		window.swap_buffers();
	}


	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
