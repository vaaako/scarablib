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


void camera_movement(Camera& camera, Keyboard& keyboard) {
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
}


bool can_move = false;
void rotate_camera(Window& window, Keyboard& keyboard, Mouse& mouse, Camera& camera) {
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

	// Press TAB to not rotate anymore
	if(can_move && keyboard.ispressed(Keycode::ESCAPE)) {
		can_move = false;
		window.hide_cursor(false);
	}
}


// TODO -- memory leak somewhere
// - Window not unloading correctly?
int main() {
	// TODO:
	// - Keyboard and Mouse public instead of a getter
	// - Mouse handle multiple inputs like keyboard
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = const_cast<char*>("Something idk"),
		.clear_color = Colors::ORANGE,
		.debug_info = true
	});

	// Load assets
	Texture tex1 = Texture("test/assets/images/hideri.jpg");
	Texture tex2 = Texture("test/assets/images/kuromi.png");

	// TODO:
	// - Remove background
	Font msgothic = Font("test/assets/fonts/msgothic.ttf", 24);

	Camera camera = Camera(window, 75.0f);

	// TODO:
	// - Instead of storing VAO object, store ID only
	Scene2D scene2d = Scene2D(window);
	Scene3D scene3d = Scene3D(window, camera);

	Cube cube = Cube();
	Cube cube2 = Cube();

	Circle circle = Circle({
		.position = { 400.0f },
		.size = { 50.0f }
	});

	Rectangle rectangle = Rectangle({
		.position = { 400.0f, 400.0f },
		.size = { 50.0f, 50.0f },
	});

	while(window.is_open()) {
		// Clear screen
		window.clear();
		// Get events
		window.process_events();

		// Handle input
		if((!can_move && window.keyboard()->ispressed(Keycode::ESCAPE)) || window.on_event(Event::QUIT)) {
			window.close();
		}

		camera_movement(camera, *window.keyboard());
		rotate_camera(window, *window.keyboard(), *window.mouse(), camera);


		// Draw font
		msgothic.set_position({ 0, 0 });
		std::stringstream stream; stream << std::setprecision(2) << window.fps();
		msgothic.set_text("FPS:" + stream.str());
		scene2d.draw_font(msgothic);

		msgothic.set_position({ 0, 24 });
		msgothic.set_text("TESTING");
		scene2d.draw_font(msgothic);

		scene3d.draw_cube(cube);
		cube2.position = { 0.0f, 0.0f, -5.0f };
		scene3d.draw_cube(cube2);

		// scene2d.draw_circle(circle);
		// scene2d.draw_rectangle(rectangle);

		window.swap_buffers();
	}

	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
