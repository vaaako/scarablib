#include "scarablib/opengl/ebo.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/scenes/scene3d.hpp"
#include "scarablib/shapes/2d/rectangle.hpp"
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
	// - Mouse handle multiple inputs like keyboard
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = const_cast<char*>("Something idk"),
		.clear_color = Colors::PINK,
		.vsync = true,
		.debug_info = true
	});

	// Load assets
	Texture tex1 = Texture("test/assets/images/kuromi.png");
	Texture tex2 = Texture("test/assets/images/purin.png");
	Texture tex3 = Texture("test/assets/images/brick.png");

	Font msgothic = Font("test/assets/fonts/msgothic.ttf", 24);

	// Make scenes
	Camera camera = Camera(window, 75.0f);
	Scene2D scene2d = Scene2D(window);
	Scene3D scene3d = Scene3D(window, camera);

	// Load mesh
	Mesh cow = Mesh("test/assets/objs/cow.obj");
	cow.set_position({ 0.0f, 0.0f, -5.0f });
	cow.set_color(Colors::CHIROYELLOW);

	// Make shapes
	// Cube position doesnt matter because will change later
	Cube cube1 = Cube({
		.position = 0.0f,
	});
	cube1.set_texture(&tex1);

	Cube cube2 = Cube({
		.position = 0.0f,
	});
	cube2.set_texture(&tex2);

	Cube cube3 = Cube({
		.position = 0.0f,
	});
	cube3.set_texture(&tex3);

	Rectangle rectangle = Rectangle({
		.position = { 400.0f, 300.0f },
		.size = { 5.0f },
	});


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
		// Until i figure out how to solve this

		// More optimized
		scene3d.draw_all({
			&cube1.set_position(vecutil::orbitate_x(cow.get_position(), rotation, 5.0f)),
			&cube2.set_position(vecutil::orbitate_y(cow.get_position(), rotation, 5.0f)),
			&cube3.set_position(vecutil::orbitate_z(cow.get_position(), -rotation, 5.0f))
		});

		scene3d.draw_mesh(cow);

		// Draw 2D shapes
		// Format FPS, ignore
		std::stringstream stream; stream << std::setprecision(2) << window.fps();
		scene2d.draw_shape(msgothic.set_text("FPS: " + stream.str()).set_position(0.0f));
		scene2d.draw_shape(msgothic.set_text("TESTING").set_position({ 0.0f, 24.0f }));

		// Aim
		scene2d.draw_shape(rectangle);


		// Update rotation
		rotation += rotation_speed;
		if(rotation >= 360.0f) {
			rotation = 0.0f; // Wrap around to keep the angle within 0-360 degrees
		}

		window.swap_buffers();
	}


	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
