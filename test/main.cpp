#include "glm/trigonometric.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/scenes/scene3d.hpp"
#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/gfx/model_factory.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/types/font.hpp"
#include "scarablib/types/skybox.hpp"
#include "scarablib/utils/math.hpp"
#include "scarablib/window/window.hpp"
#include "scarablib/input/keycode.hpp"
#include <cstdio>
#include <iostream>

bool can_move = false;

void camera_movement(Window& window, Camera& camera, KeyboardHandler& keyboard) {
	const float dt = window.dt();

	if(keyboard.isdown(Keycode::W)) {
		camera.move_front(dt);
	} else if (keyboard.isdown(Keycode::S)) {
		camera.move_back(dt);
	}

	if(keyboard.isdown(Keycode::A)) {
		camera.move_left(dt);
	} else if (keyboard.isdown(Keycode::D)) {
		camera.move_right(dt);
	}

	if(keyboard.isdown(Keycode::SPACE)) {
		camera.fly_up(dt);
	} else if(keyboard.isdown(Keycode::LSHIFT)) {
		camera.fly_down(dt);
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
		// Prevent initial jump
		mouse.set_cursor_position(window, window.get_half_width(), window.get_half_height());
		window.hide_cursor(true);
		return; // Prevent camera from look where the cursor clicked
	}

	// Rotate camera
	if(can_move) {
		camera.rotate(mouse);
		mouse.set_cursor_position(window, window.get_half_width(), window.get_half_height()); // Prevent cursor from exiting the screen
	}
}


// TODO: move to some class/struct
// Not finished yet, just for tests
bool is_aabb(Model& model1, Model& model2) {
	const vec3<float>& modelmin = model1.get_min();
	const vec3<float>& modelmax = model1.get_max();

	const vec3<float>& model2min = model2.get_min();
	const vec3<float>& model2max = model2.get_max();

	return modelmin.x <= model2max.x &&
		modelmax.x >= model2min.x &&

		modelmin.y <= model2max.y &&
		modelmax.y >= model2min.y &&

		modelmin.z <= model2max.z &&
		modelmax.z >= model2min.z;
}



// RAM usage [Approximated values] (~140mb total in this example):
// Tested using 16Gb RAM, Xeon E5 2620 v3 and RX450
// - Window: ~126mb
//    + OpenGL: ~10mb to ~15mb
//    + SDL: ~12mb
//    + "SDL_WINDOW_OPENGL" flag: ~100mb (really, just this flag)
//
// This flag also slows the time take to create a window speed

int main() {
	constexpr float DT_MODIFIER = 100.0f; // Multipling by 100 because of delta (since it's in ms)

	// TODO:
	// - Mouse handle multiple inputs like keyboard
	// NOTE -- ~113mb here
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = const_cast<char*>("Something idk"),
		.vsync = false,
		.debug_info = true
	});
	window.set_clear_color(Colors::PINK);

	// Load assets
	Texture tex1 = Texture("test/assets/images/kuromi.png");
	Texture tex2 = Texture("test/assets/images/purin.png");
	Texture tex3 = Texture("test/assets/images/brick.png");
	Texture snail = Texture("test/assets/images/snail.png");

	// TODO: Make it use only one drawcall
	Font msgothic = Font("test/assets/fonts/Ubuntu-R.ttf", 24);

	// Make scenes
	Camera camera = Camera(window, 75.0f);
	camera.set_speed(1.0f * DT_MODIFIER);


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
	Model* cow = new Model("test/assets/objs/cow.obj");
	cow->set_position({ 0.0f, 0.0f, -5.0f });
	cow->set_color(Colors::CHIROYELLOW);
	cow->set_orientation(90.0f, { false, false, true });
	scene3d.add_to_scene("cow", cow);

	// Make shapes
	// Cube position doenst matter because will change later
	Cube* cube = ModelFactory::create_cube({});
	scene3d.add_to_scene("cube1", cube);
	cube->set_texture(&tex1);
	// std::shared_ptr<Cube> cubefrom = scene3d.get_by_key<Cube>("cube1");

	Cube* cube2 = ModelFactory::create_cube({});
	cube2->set_texture(&tex2);
	scene3d.add_to_scene("cube2", cube2);

	Cube* cube3 = ModelFactory::create_cube({});
	cube3->set_texture(&tex3);
	scene3d.add_to_scene("cube3", cube3);

	// Cube* camera_hitbox = ModelFactory::create_cube({
	// 	.position = vec3<float>(-10.0f, 1.0f, -10.0f)
	// });
	// Dont add to scene to not draw it
	// scene3d.add_to_scene("cameracollision", camera_hitbox);


	Plane* plane = ModelFactory::create_plane({
		.position = vec3<float>(-5.0f, 1.0f, -10.0f),
		// Z doens't matter
		.scale = vec3<float>(2.0f, 2.0f, 0.0f),
	});
	plane->set_texture(&snail);
	scene3d.add_to_scene("plane", plane);


	// Rectangle* rectangle = new Rectangle({
	// 	.position = vec2<uint32>(
	// 		window.get_half_width()  - 10,
	// 		window.get_half_height() - 10
	// 	),
	// 	.size = vec2<float>(10.0f, 10.0f)
	// });
	// scene2d.add_to_scene("rectangle", rectangle);

	LOG_INFO("Scene3d length %d", scene3d.length());

	bool debug_mode = false;
	uint32 timer = window.timenow(); // Start showing (because will decrease itself at start)

	float rotation = 0.0f;
	float rotation_speed = 1.0f * DT_MODIFIER;
	
	while(window.is_open()) {
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

		// Just works with cubes and planes yet
		// collision = false;
		// camera_hitbox->set_position(camera.get_position());
		// if(is_aabb(*camera_hitbox, *cow)) {
		// 	collision = true;
		// }


		// WARNING: When drawing 3D and 2D shapes together, draw 3D shapes first

		// Rotate models
		vec3<float> cowpos = cow->get_position();
		cow->set_rotation(rotation, { true, true, true });
		cube->set_position(ScarabMath::orbitate_x(cowpos, -rotation, 5.0f));
		cube2->set_position(ScarabMath::orbitate_y(cowpos, rotation, 5.0f));
		cube3->set_position(ScarabMath::orbitate_z(cowpos, -rotation, 5.0f));

		// Rotate plane
		plane->face_position(camera.get_position());

		// Draw
		skybox.draw();
		scene3d.draw_all();



		// Toggle debug mode
		if (window.keyboard().ispressed(Keycode::F3)) {
			debug_mode = !debug_mode;
		}

		if(debug_mode) {
			msgothic.draw_text("FPS: " + std::to_string(window.fps()), { 0.0f, 0.0f }, Colors::WHITE, 1.0f);

			// Elapsed 1 second
			uint32 current = window.timenow();
			if(current - timer >= 1000) {
				timer = current; // Reset timer
				std::cout << window.dt() << std::endl;
			}
		}


		// Draw 2D shapes
		// msgothic.draw_text("COLLISION: " + std::to_string(collision), { 0.0f, 24.0f });
		// msgothic.draw_text("X: " + std::to_string(camera.get_x()) + ", "
		// 		+ "Y: " + std::to_string(camera.get_y()) + ", "
		// 		+ "Z: " + std::to_string(camera.get_z()), { 0.0f, 24.0f });
		// scene2d.draw_all();

		// Update rotation
		rotation += rotation_speed * window.dt();
		rotation = std::fmod(rotation, 360.0f); // Wrap around (if goes above 360.0f change to 0.0f) (if goes above 360.0f change to 0.0f)

		// window.frame_capping(75);
		window.swap_buffers(); // NOTE -- ~14mb RAM
	}


	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("OpenGL error code: %x", err);
	}
}
