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
#include "scarablib/gfx/skybox.hpp"
#include "scarablib/types/texture_array.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/utils/math.hpp"
#include "scarablib/window/window.hpp"
#include "scarablib/input/keycode.hpp"
#include <cstdio>
#include <iostream>

#ifdef SCARAB_IMGUI
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#endif

bool mouse_captured = false;

void camera_movement(Window& window, Camera& camera, KeyboardHandler& keyboard) {
	const float dt = window.dt();
	// LOG_DEBUG("Camera dt: %f", dt);

	vec3<float> direction;

	if(keyboard.isdown(Keycode::W)) {
		direction.x = 1.0f;
	} else if (keyboard.isdown(Keycode::S)) {
		direction.x = -1.0f;
	}

	if(keyboard.isdown(Keycode::A)) {
		direction.z = 1.0f;
	} else if (keyboard.isdown(Keycode::D)) {
		direction.z = -1.0f;
	}

	if(keyboard.isdown(Keycode::SPACE)) {
		direction.y = 1.0f;
	} else if(keyboard.isdown(Keycode::LSHIFT)) {
		direction.y = -1.0f;
	}

	// FIX: Diagonal movement acceleration
	camera.move(direction, dt);

	if(mouse_captured && keyboard.ispressed(Keycode::ESCAPE)) {
		mouse_captured = false;
		window.grab_cursor(false);
	}
}

void rotate_camera(Window& window, Camera& camera, MouseHandler& mouse) {
	#ifdef SCARAB_IMGUI
	// Ignore if mouse event was in imgui window
	if (ImGui::GetIO().WantCaptureMouse) {
		return;
	}
	#endif

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


// RAM usage [Approximated values] (~140mb total in this example):
// Tested using 16Gb RAM, Xeon E5 2620 v3 and RX450
// - Window: ~126mb
//    + OpenGL: ~10mb to ~15mb
//    + SDL: ~12mb
//    + "SDL_WINDOW_OPENGL" flag: ~100mb (really, just this flag)
//
// This flag also slows the time take to create a window speed

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

	// TODO: Make it use only one drawcall
	// WARNING: FONTS ARE NOT COMPLETED YET, I WILL UPDATE IT LATER
	Font msgothic = Font("test/assets/fonts/Ubuntu-R.ttf", 24);

	// Make scenes
	Camera camera = Camera(window, 75.0f, 0.1f);
	camera.set_speed(1.0f * DELTATIME_MODIFIER);


	Scene2D scene2d = Scene2D(window);
	Scene3D scene3d = Scene3D(window, camera);
	// scene3d.show_bounding_box(true);

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

	// BUG: Memory leak somewhere in billboard class
	Billboard* bill = ModelFactory::create_billboard({
		.position = vec3<float>(-5.0f, 1.0f, -10.0f),
		.scale = vec3<float>(4.0f),
	});

	// // Front-right and Right are unecessary since they are the same texture but flipped
	bill->set_directional_textures({
		"test/assets/images/directions/pinky/0.png", // 1
		"test/assets/images/directions/pinky/1.png", // 2
		"test/assets/images/directions/pinky/2.png", // 3
		"test/assets/images/directions/pinky/3.png", // 4
		"test/assets/images/directions/pinky/4.png"  // 5
	}, 234); // Flip textures to opposite

	scene3d.add_to_scene("bill", bill);
	// bill->show_bounding_box(true);

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
	bool envsync = true;
	uint32 timer = window.timenow(); // Start showing (because will decrease itself at start)

	float rotation = 0.0f;
	float rotation_speed = 1.0f * DELTATIME_MODIFIER;

	// TODO: Bounding box collision not tested
	while(window.is_open()) {
		window.clear(); // NOTE -- ~14mb RAM itself
		window.process_events(); // Process all events

		// Quit if ESC or Q is pressed
		if((!mouse_captured && window.keyboard().ispressed(Keycode::ESCAPE)) || window.has_event(Event::QUIT)) {
			window.close();
		}

		// Resize support
		window.on_event(Event::WINDOW_RESIZED, [&](){
			camera.update_viewport(window);
		});

		// Handle camera keyboard inputs
		camera_movement(window, camera, window.keyboard());
		rotate_camera(window, camera, window.mouse());

		// WARNING: When drawing 3D and 2D shapes together, draw 3D shapes first

		// Rotate models
		vec3<float> cowpos = cow->get_position();
		// cow->set_rotation(rotation, { true, true, true });
		cube->set_position(ScarabMath::orbitate_x(cowpos, -rotation, 5.0f));
		cube2->set_position(ScarabMath::orbitate_y(cowpos, rotation, 5.0f));
		cube3->set_position(ScarabMath::orbitate_z(cowpos, -rotation, 5.0f));

		bill->rotate_eight_directions(camera.get_position());

		// Draw
		skybox.draw();
		scene3d.draw_all();

		// Toggle debug mode
		if (window.keyboard().ispressed(Keycode::F3)) {
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
				LOG_INFO("Camera position: %f, %f, %f", camera.get_x(), camera.get_y(), camera.get_z());
				LOG_DEBUG("Delta time: %f", window.dt());
			}
		}

		// Elapsed 1 second
		uint32 current = window.timenow();
		if(current - timer >= 1000) {
			timer = current; // Reset timer
			window.set_title("FPS: " + std::to_string(window.fps()) + " / Delta: " + std::to_string(window.dt()));
		}


		// Draw 2D shapes
		// msgothic.draw_text("COLLISION: " + std::to_string(collision), { 0.0f, 24.0f });
		// msgothic.draw_text("X: " + std::to_string(camera.get_x()) + ", "
		// 		+ "Y: " + std::to_string(camera.get_y()) + ", "
		// 		+ "Z: " + std::to_string(camera.get_z()), { 0.0f, 24.0f });
		// scene2d.draw_all();

		#ifdef SCARAB_IMGUI
		// ImGui_ImplOpenGL3_NewFrame();
		// ImGui_ImplSDL2_NewFrame();
		// ImGui::NewFrame();
		//
		// // Create a simple ImGui window

// chiro 🌸
		// ImGui::Begin("Hello, ImGui!");
		// ImGui::Text("This is a simple ImGui window.");
		// if (ImGui::Button("Click Me")) {
		// 	printf("Button clicked!\n");
		// }
		// ImGui::End();
		// #endif
		//
		// #ifdef SCARAB_IMGUI
		// // Render ImGui
		// ImGui::Render();
		// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		#endif

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
