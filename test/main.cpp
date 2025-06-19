#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/skybox.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/scenes/scene3d.hpp"
#include "scarablib/window/window.hpp"

void camera_movement(Window& window, Camera& camera, const float dt) {
	static bool mouse_captured = false;

	vec3<int8> direction = vec3<int8>(0.0f);

	if(window.iskeydown(Keycode::W)) {
		direction.z = 1;
	} else if (window.iskeydown(Keycode::S)) {
		direction.z = -1;
	}

	if(window.iskeydown(Keycode::A)) {
		direction.x = -1;
	} else if (window.iskeydown(Keycode::D)) {
		direction.x = 1;
	}

	if(window.iskeydown(Keycode::SPACE)) {
		direction.y = 1;
	} else if(window.iskeydown(Keycode::LSHIFT)) {
		direction.y = -1;
	}

	// FIX: Diagonal movement acceleration
	camera.move(direction, dt);

	// ROTATION LOGIC //
	// Only rotate when click on screen
	if(!mouse_captured && window.isbtndown(Buttoncode::LMB)) {
		LOG_INFO("Clicked");
		mouse_captured = true;
		window.grab_cursor(true);
	}

	// Rotate camera
	if(mouse_captured) {
		camera.rotate(window);
		if(window.iskeypressed(Keycode::ESCAPE) || window.iskeypressed(Keycode::TAB)) {
			mouse_captured = false;
			window.grab_cursor(false);
		}
	}
}

void update_gravity(Model& model, const float dt, const float ground_y = 0.0f) {
	if(model.physics->on_ground) {
		return;
	}

	// Apply earth's gravity
	model.physics->velocity.y += -9.8f * dt; // Since it accumulates, longer it falls the faster

	// Move model
	model.set_position(model.get_position() + model.physics->velocity * dt);

	// Collision check with ground
	if(model.get_position().y <= ground_y) {
		// Snap to ground
		model.set_y(ground_y);

		// Setop falling
		model.physics->velocity.y = 0.0f;
		model.physics->on_ground = true;
	}
}

// TODO: Physics component
// MEMORY LEAK SOMEWHERE

int main() {
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = "FPS: 0",
		.vsync = true,
		.resizable = true,
		.debug_info = true
	});

	// CAMERAS AND SCENES //

	Camera camera = Camera(window);
	camera.set_speed(200.0f);

	Camera2D camera2d = Camera2D(window);

	Scene2D scene2d = Scene2D(camera2d);
	Scene3D scene3d = Scene3D(camera);

	// CAMERAS AND SCENES //

	// ASSETS //
	Texture kuromi = Texture("test/assets/images/kuromi.png");

	Skybox skybox = Skybox(camera, {
		"test/assets/images/skybox/right.jpg",
		"test/assets/images/skybox/left.jpg",
		"test/assets/images/skybox/top.jpg",
		"test/assets/images/skybox/bottom.jpg",
		"test/assets/images/skybox/front.jpg",
		"test/assets/images/skybox/back.jpg"
	});

	// ASSETS //

	// MODELS //

	Model* cow = scene3d.add<Model>("cow", "test/assets/objs/cow.obj");
	cow->material.color = Colors::CHIROYELLOW;
	cow->set_position({ 0.0f, 0.0f, -20.0f });
	cow->set_orientation(90.0f, { false, false, true });
	// cow->update_bbox(); // Update local bounds
	cow->set_dynamic_bbox_update(true); // Update bounding box with the model matrix

	Rectangle* rect = scene2d.add<Rectangle>("rect");
	rect->material.color = Colors::RED;
	rect->set_position({ 100.0f, 100.0f });
	rect->set_size({ 50.0f, 50.0f });

	Cube* cube = scene3d.add<Cube>("cube", Cube::Face::FRONT | Cube::Face::BACK);
	cube->material.texture = &kuromi;
	cube->set_position({ 0.0f, 0.0f, -5.0f });
	cube->set_scale(vec3<float>(2.0f));
	cube->enable_physics(1.0f, true);
	// Then PhysicsComponent is nullptr until enabled
	// Physics component may have apply_gravity member
	// Scene3D may have a update_physics method that calls all physics components of all models in map

	Billboard* bill = scene3d.add<Billboard>("bill");
	bill->set_position({ -5.0f, 1.0f, -10.0f });
	bill->set_scale(vec4<float>(4.0f));
	bill->set_directional_textures({
		"test/assets/images/directions/pinky/0.png", // FRONT
		"test/assets/images/directions/pinky/1.png", // FRONTRIGHT
		"test/assets/images/directions/pinky/2.png", // RIGHT
		"test/assets/images/directions/pinky/3.png", // BACKRIGHT
		"test/assets/images/directions/pinky/4.png"  // BACK
	}, Billboard::FRONTRIGHT | Billboard::RIGHT | Billboard::BACKRIGHT); // Flip textures


	// scene3d.remove_by_key("cube");
	// scene2d.remove_by_key("rect");

	// MODELS //

	float dt = 0.0f;
	float angle = 0.0f;
	bool should_fall = false;

	while(window.is_open()) {
		window.clear(); // Draw start

		// WINDOW EVENTS //
		window.process_events();

		dt = window.dt();

		window.on_event(Event::WINDOW_RESIZED, [&]() {
			camera.update_viewport(window);
			camera2d.update_viewport(window);
		});

		if(window.has_event(Event::WINDOW_QUIT) || window.iskeypressed(Keycode::ESCAPE)) {
			window.close();
		}

		if(window.iskeypressed(Keycode::F)) {
			cube->physics->on_ground = false;
			cube->set_y(100.0f);
			should_fall = true;
		}

		// LOG_DEBUG("Camera Position: (%f, %f, %f)", camera.position.x, camera.position.y, camera.position.z);

		window.set_title("FPS: " + std::to_string(window.fps()));
		// WINDOW EVENTS //

		// MODELS UPDATE //
		bill->update_facing_texture(camera.position);

		cow->set_rotation(angle, { true, false, true });

		if(should_fall) {
			update_gravity(*cube, dt);
			if(cube->get_position().y == 0.0f) {
				should_fall = false;
			}
		}

		// Debug draw bounds
		cow->bbox.draw_local_bounds(camera, Colors::GREEN, true);
		cow->bbox.draw_world_bounds(camera, Colors::RED, false);
		// MODELS UPDATE //

		// OTHER EVENTS //
		camera_movement(window, camera, dt);

		skybox.draw();
		scene2d.draw_all();
		scene3d.draw_all();

		if(angle >= 360.0f) {
			angle = 0.0f;
		}

		angle += 90.0f * dt; // 90 degrees per second
		// OTHER EVENTS //

		window.swap_buffers(); // Draw end
	}
}
