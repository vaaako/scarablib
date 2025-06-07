#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/scenes/scene3d.hpp"
#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/gfx/skybox.hpp"
#include "scarablib/types/font.hpp"
#include "scarablib/types/texture_array.hpp"
#include "scarablib/utils/math.hpp"
#include "scarablib/window/window.hpp"
#include "scarablib/input/keycode.hpp"
#include "scarablib/network/network.hpp"
#include <cstdio>

#include <sys/sysinfo.h>
#include <sys/resource.h>

bool mouse_captured = false;

void camera_movement(Window& window, Camera& camera, KeyboardHandler& keyboard) {
	const float dt = window.dt();
	// LOG_DEBUG("Camera dt: %f", dt);

	vec3<int8> direction = vec3<int8>(0.0f);

	if(keyboard.isdown(Keycode::W)) {
		direction.z = 1;
	} else if (keyboard.isdown(Keycode::S)) {
		direction.z = -1;
	}

	if(keyboard.isdown(Keycode::A)) {
		direction.x = -1;
	} else if (keyboard.isdown(Keycode::D)) {
		direction.x = 1;
	}

	if(keyboard.isdown(Keycode::SPACE)) {
		direction.y = 1;
	} else if(keyboard.isdown(Keycode::LSHIFT)) {
		direction.y = -1;
	}

	// FIX: Diagonal movement acceleration
	camera.move(direction, dt);

	if(mouse_captured && keyboard.ispressed(Keycode::ESCAPE)) {
		mouse_captured = false;
		window.grab_cursor(false);
	}
}

void rotate_camera(Window& window, Camera& camera, MouseHandler& mouse) {
	// Ignore if mouse event was in imgui window
	// if (ImGui::GetIO().WantCaptureMouse) {
	// 	return;
	// }

	// ROTATION LOGIC //
	// Only rotate when click on screen
	if(!mouse_captured && mouse.isclick(MouseHandler::Button::LMB)) {
		LOG_INFO("Clicked");
		mouse_captured = true;
		window.grab_cursor(true);
	}

	// Rotate camera
	if(mouse_captured) {
		camera.rotate(mouse);
	}
}


// RAM usage [Approximated values]
// Tested using 16Gb RAM, Xeon E5 2620 v3 and RX550
//
// RAM usage: ~160mb -> Main loop / Clear / Swap Buffer / Process events
// - Window: ~126mb - Some of this may be the storage of float values and event map
//    + OpenGL: ~10mb to ~15mb
//    + SDL: ~12mb
//    + "SDL_WINDOW_OPENGL" flag: ~100mb (really, just this flag)
//
// This flag also slows the time it takes to create a window

int main() {
	constexpr float DELTATIME_MODIFIER = 100.0f; // Multipling by 100 because of delta (since it's in ms)

	// NOTE -- ~113mb RAM here
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = "FPS: 0",
		.vsync = true,
		.resizable = true,
		.debug_info = true
	});
	window.set_clear_color(Colors::PINK);
	// window.set_border(false);

	// Load assets
	Texture tex1  = Texture({ .path = "test/assets/images/kuromi.png" });
	Texture tex2  = Texture({ .path = "test/assets/images/purin.png" });
	Texture tex3  = Texture({ .path = "test/assets/images/brick.png" });
	Texture snail = Texture({ .path = "test/assets/images/snail.png" });

	// Make scenes
	Camera camera = Camera(window, 75.0f, 0.1f);
	camera.set_speed(1.0f * DELTATIME_MODIFIER);

	Camera2D camera2d = Camera2D(window);

	Scene2D scene2d = Scene2D(camera2d);
	Scene3D scene3d = Scene3D(camera);

	// WARNING: FONTS ARE NOT COMPLETED YET, I WILL UPDATE IT LATER
	Font msgothic = Font(camera2d, "test/assets/fonts/msgothic.ttf", 32);

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

	cow->set_transform();
	// cow->set_dynamic_transform(true);

	// Make shapecollision
	Cube* cube = new Cube();
	scene3d.add_to_scene("cube1", cube);
	cube->set_position({ -10.0f, 1.0f, -10.0f }); // Cube position doesnt matter because will change later
	cube->set_texture(&tex1);

	Cube* cube2 = new Cube();
	cube2->set_texture(&tex2);
	scene3d.add_to_scene("cube2", cube2);

	Cube* cube3 = new Cube();
	cube3->set_texture(&tex3);
	scene3d.add_to_scene("cube3", cube3);

	// BUG: Memory leak somewhere in billboard class
	Billboard* bill = new Billboard();
	bill->set_position({ -5.0f, 1.0f, -10.0f });
	bill->set_scale(vec4<float>(4.0f));
	// Front-right and Right are unecessary since they are the same texture but flipped
	bill->config_directional_textures({
		"test/assets/images/directions/pinky/0.png", // 1
		"test/assets/images/directions/pinky/1.png", // 2
		"test/assets/images/directions/pinky/2.png", // 3
		"test/assets/images/directions/pinky/3.png", // 4
		"test/assets/images/directions/pinky/4.png"  // 5
	}, 234); // Flip textures to opposite

	scene3d.add_to_scene("bill", bill);

	// Cube* testcube = ModelFactory::create_cube();
	// testcube->set_scale(vec3<float>(0.5f));
	// testcube->set_position(cow->get_center_position());
	// scene3d.add_to_scene("testcube", testcube);

	Rectangle* sprite = new Rectangle();
	sprite->set_position({ 10.0f, 10.0f });
	sprite->set_size({ 100.0f, 100.0f });
	sprite->set_color(Colors::PINK);
	sprite->set_texture(&tex1);
	scene2d.add_to_scene("rec", sprite);

	LOG_INFO("Scene3d length %d", scene3d.length());

	// Testing get by key (works)
	// auto cube1_later = scene3d.get_by_key<Cube>("cube1");
	// cube1_later->set_color(Colors::CHIROYELLOW);


	// Network server = Network(7777, 32, true);
	// server.threaded_gather_events(1000, [](std::vector<Network::Packet> packets){
	// 	if(packets.empty()) {
	// 		LOG_INFO("no packets");
	// 	}
	// });

	bool debug_mode = false;
	bool envsync = true;
	uint32 timer = window.timenow(); // Start showing (because will decrease itself at start)

	float rotation = 0.0f;
	float rotation_speed = 1.0f * DELTATIME_MODIFIER;


	window.set_cullface(false);

	// TODO: Bounding box collision not tested
	while(window.is_open()) {
		window.clear(); // NOTE -- ~14mb RAM itself
		window.process_events(); // Process all events

		// Quit if ESC or Q is pressed
		if((!mouse_captured && window.keyboard().ispressed(Keycode::ESCAPE)) || window.has_event(Event::WINDOW_QUIT)) {
			window.close();
		}

		// Resize support
		// (OpenGL viewport is updated automatically)
		window.on_event(Event::WINDOW_RESIZED, [&]() {
			scene2d.update_viewport(window);
			scene3d.update_viewport(window);
		});

		// Handle camera keyboard inputs
		camera_movement(window, camera, window.keyboard());
		rotate_camera(window, camera, window.mouse());

		// WARNING: When drawing 3D and 2D shapes together, draw 3D shapes first

		// Rotate models
		vec3<float> cowpos = cow->get_position();
		cow->set_rotation(rotation, { true, true, true });
		// cube->set_position(ScarabMath::orbitate_x(cowpos, -rotation, 5.0f));
		cube2->set_position(ScarabMath::orbitate_y(cowpos, rotation, 5.0f));
		cube3->set_position(ScarabMath::orbitate_z(cowpos, -rotation, 5.0f));

		bill->update_facing_texture(camera.position);
		// LOG_DEBUG("Cardinal: %s", camera.get_cardinal_direction().c_str());

		cow->draw_collider(camera, Colors::RED, false);

		// Draw
		skybox.draw();
		scene3d.draw_all();

		// Toggle debug mode
		if(window.keyboard().ispressed(Keycode::F3)) {
			debug_mode = !debug_mode;
		}

		if(window.keyboard().ispressed(Keycode::V)) {
			envsync = !envsync;
			LOG_INFO("VSync: %d", envsync);
			window.set_vsync(envsync);
		}

		if(debug_mode) {
			// msgothic.draw_text("FPS: " + std::to_string(window.fps()), { 0.0f, 0.0f }, Colors::WHITE, 1.0f);

			// Elapsed 1 second
			uint32 current = window.timenow();
			if(current - timer >= 1000) {
				timer = current; // Reset timer
				LOG_INFO("Camera position: %f, %f, %f", camera.position.x, camera.position.y, camera.position.z);
				LOG_DEBUG("Delta time: %f", window.dt());
			}
		}

		// Elapsed 1 second
		uint32 current = window.timenow();
		if(current - timer >= 1000) {
			timer = current; // Reset timer
			window.set_title("FPS: " + std::to_string(window.fps()) + " / Delta: " + std::to_string(window.dt()));
		}


		// msgothic.draw_text("Hello world", { 10.0f, 10.0f });
		// msgothic.draw_all(); // drop fps i think
		scene2d.draw_all();

		// Update rotation
		rotation += rotation_speed * window.dt();
		rotation = std::fmod(rotation, 360.0f); // Wrap around (if goes above 360.0f change to 0.0f) (if goes above 360.0f change to 0.0f)

		// window.frame_capping(60.0f);
		window.swap_buffers(); // NOTE -- ~14mb RAM
	}

	// Check for any OpenGL error
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		LOG_ERROR("\nOpenGL error code: %x", err);
	}
}
