#include "glm/trigonometric.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/scenes/scene3d.hpp"
#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/types/font.hpp"
#include "scarablib/types/skybox.hpp"
#include "scarablib/utils/math.hpp"
#include "scarablib/window/window.hpp"
#include "scarablib/input/keycode.hpp"
#include <cstdio>

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



bool is_aabb(Mesh& mesh1, Mesh& mesh2) {
	const vec3<float>& meshmin = mesh1.get_min();
	const vec3<float>& meshmax = mesh1.get_max();

	const vec3<float>& mesh2min = mesh2.get_min();
	const vec3<float>& mesh2max = mesh2.get_max();

	return meshmin.x <= mesh2max.x &&
		meshmax.x >= mesh2min.x &&

		meshmin.y <= mesh2max.y &&
		meshmax.y >= mesh2min.y &&

		meshmin.z <= mesh2max.z &&
		meshmax.z >= mesh2min.z;
}



// TODO -- memory leak somewhere
// - Window not unloading correctly?
// - stbi maybe?
//
// RAM usage [Approximated values] (~140mb total in this example):
// Tested using 16Gb RAM, Xeon E5 2620 v3 and RX450
// - Window: ~126mb
//    + OpenGL: ~10mb to ~15mb
//    + SDL: ~12mb
//    + "SDL_WINDOW_OPENGL" flag: ~100mb (really, just this flag)
//
// This flag also slows the time take to create a window speed

// TODO -- Fix 2D and 3D order to any order (DEPTH_TEST problem)

int main() {
	// TODO:
	// - Mouse handle multiple inputs like keyboard
	// NOTE -- ~113mb here
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = const_cast<char*>("Something idk"),
		.vsync = true,
		.debug_info = true
	});
	window.set_clear_color(Colors::PINK);

	// Load assets
	Texture tex1 = Texture("test/assets/images/kuromi.png");
	Texture tex2 = Texture("test/assets/images/purin.png");
	Texture tex3 = Texture("test/assets/images/brick.png");
	Texture snail = Texture("test/assets/images/snail.png");

	Font msgothic = Font("test/assets/fonts/msgothic.ttf", 24);

	// Make scenes
	Camera camera = Camera(window, 75.0f);
	camera.set_speed(1.5f);


	Scene2D scene2d = Scene2D(window);
	Scene3D scene3d = Scene3D(window, camera);

	Skybox skybox = Skybox(camera, {
		"test/assets/images/skybox/right.jpg",
		"test/assets/images/skybox/left.jpg",
		"test/assets/images/skybox/top.jpg",
		"test/assets/images/skybox/bottom.jpg",
		"test/assets/images/skybox/front.jpg",
		"test/assets/images/skybox/back.jpg"
	});

	// Load mesh
	Mesh cow = Mesh("test/assets/objs/cube.obj");
	cow.set_position({ 0.0f, 0.0f, -5.0f });
	cow.set_color(Colors::CHIROYELLOW);

	cow.set_orientation(90.0f, { false, false, true });

	// Make shapes
	// Cube position doesnt matter because will change later
	Cube cube1 = Cube({
		.position = vec3<float>(0.0f),
	});
	cube1.set_texture(&tex1);

	Cube cube2 = Cube({
		.position = vec3<float>(0.0f),
	});
	cube2.set_texture(&tex2);

	Cube cube3 = Cube({
		.position = vec3<float>(0.0f),
	});
	cube3.set_texture(&tex3);


	Cube cameracollision = Cube({
		.position = camera.get_position(),
	});


	Plane plane = Plane({
		.position = vec3<float>(-5.0f, 1.0f, -10.0f),
		// Z doens't matter
		.scale = vec3<float>(2.0f, 2.0f, 0.0f),
	});
	plane.set_texture(&snail);



	float rotation = 0.0f;
	float rotation_speed = 1.0f;

	bool collision = false;
	while(window.is_open()) {
		// Clear screen
		window.clear(); // NOTE -- ~14mb RAM itself
		// Get events
		window.process_events();

		// Handle input
		if((!can_move && window.keyboard().ispressed(Keycode::ESCAPE)) || window.on_event(Event::QUIT)) {
			window.close();
		}

		// Handle camera keyboard inputs
		camera_movement(window, camera, window.keyboard());
		rotate_camera(window, camera, window.mouse());

		collision = false;
		cameracollision.set_position(camera.get_position());
		if(is_aabb(cameracollision, plane)) {
			collision = true;
		}


		// WARNING -- When drawing 3D and 2D shapes together, draw 3D shapes first
		// Until i figure out how to solve this

		skybox.draw();
		scene3d.draw_mesh(cow.set_rotation(rotation, vec3<bool>(true, false, false)));

		// More optimized drawing for the same shape
		scene3d.draw_all({
			&cube1.set_position(ScarabMath::orbitate_x(cow.get_position(), -rotation, 5.0f)),
			&cube2.set_position(ScarabMath::orbitate_y(cow.get_position(), rotation, 5.0f)),
			&cube3.set_position(ScarabMath::orbitate_z(cow.get_position(), -rotation, 5.0f))
		});

		plane.face_position(camera.get_position());
		scene3d.draw_mesh(plane);


		// Draw 2D shapes
		// Format FPS, ignore
		std::stringstream stream; stream << std::setprecision(2) << window.fps();
		scene2d.draw_shape(msgothic.set_text("FPS: " + stream.str()).set_position(vec3<float>(0.0f)));

		// scene2d.draw_shape(msgothic.set_text("COLLISION: " + std::to_string(collision)).set_position(vec2<float>(0.0f, 24.0f)));
		scene2d.draw_shape(msgothic.set_text("POS: "
					+ std::to_string(camera.get_x()) + ", "
					+ std::to_string(camera.get_y()) + ", "
					+ std::to_string(camera.get_z()))
					.set_position(vec2<float>(0.0f, 24.0f)));

		// Update rotation
		rotation += rotation_speed;
		if(rotation >= 360.0f) {
			rotation = 0.0f; // Wrap around to keep the angle within 0-360 degrees
		}

		window.swap_buffers(); // NOTE -- ~14mb RAM
	}


	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %d", err);
	}
}
