#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/gfx/3d/cube.hpp"
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

	// if (direction.length_squared() > 0.0f) {
	// 	direction = normalize(direction);
	// 	camera.move(direction, dt);
	// }

	// ROTATION LOGIC //
	// Only rotate when click on screen
	if(!mouse_captured && window.isclick(Buttoncode::LMB)) {
		LOG_INFO("Clicked");
		mouse_captured = true;
		window.grab_cursor(true);
	}

	// Rotate camera
	if(mouse_captured) {
		camera.rotate(window);
		if(window.iskeypressed(Keycode::ESCAPE)) {
			mouse_captured = false;
			window.grab_cursor(false);
		}
	}
}

void update_gravity(Model& model, const float dt, const float ground_y = 0.0f) {
	if(model.physics.on_ground) {
		return;
	}

	// Apply earth's gravity
	model.physics.velocity.y += -9.8f * dt; // Since it accumulates, longer it falls the faster

	// Move model
	model.set_position(model.get_position() + model.physics.velocity * dt);

	// Collision check with ground
	if(model.get_position().y <= ground_y) {
		// Snap to ground
		model.set_y(ground_y);

		// Setop falling
		model.physics.velocity.y = 0.0f;
		model.physics.on_ground = true;
	}
}

// TODO: Physics component

int main() {
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = "FPS: 0",
		.vsync = true,
		.resizable = true,
		.debug_info = true
	});

	Camera camera = Camera(window);
	// camera.position = { -1.0f, 20.0f, 60.0f };
	camera.set_speed(1.0f * 100.0f);

	Camera2D camera2d = Camera2D(window);

	Scene2D scene2d = Scene2D(camera2d);
	Scene3D scene3d = Scene3D(camera);

	Model* cow = scene3d.add<Model>("cow", "test/assets/objs/cow.obj");
	cow->set_position({ 0.0f, 0.0f, -20.0f });
	cow->set_color(Colors::CHIROYELLOW);
	cow->set_orientation(90.0f, { false, false, true });
	cow->set_transform(); // Apply changes to the bounding box
	// cow->set_dynamic_transform(true); // Update bounding box with the model matrix

	Rectangle* rect = scene2d.add<Rectangle>("rect");
	rect->set_position({ 100.0f, 100.0f });
	rect->set_size({ 50.0f, 50.0f });
	rect->set_color(Colors::RED);

	Cube* cube = scene3d.add<Cube>("cube", Cube::Face::FRONT | Cube::Face::BACK);
	cube->set_position({ 0.0f, 0.0f, -5.0f });
	cube->set_scale(vec3<float>(2.0f));
	// cube->enable_physics(mass: 1.0f, gravity: true)
	// Then PhysicsComponent is nullptr until enabled
	// Physics component may have apply_gravity member
	// Scene3D may have a update_physics method that calls all physics components of all models in map

	float dt = 0.0f;
	float angle = 0.0f;

	float accumulator = 0.0f;
	const float FIXED_DT = 1.0f / 75.0f;

	while(window.is_open()) {
		window.clear();
		window.process_events();

		dt = window.dt();
		accumulator += dt;

		window.on_event(Event::WINDOW_RESIZED, [&]() {
			camera.update_viewport(window);
			camera2d.update_viewport(window);
		});

		if(window.has_event(Event::WINDOW_QUIT) || window.iskeypressed(Keycode::ESCAPE)) {
			window.close();
		}

		// LOG_DEBUG("Cube position: %f %f %f", cube->get_position().x, cube->get_position().y, cube->get_position().z);
		window.set_title("FPS: " + std::to_string(window.fps()));

		cow->set_rotation(angle, { true, false, true });
		cow->draw_collider(camera, Colors::RED, false);

		while(accumulator >= FIXED_DT) {
			// update_gravity(*cube, FIXED_DT);
			camera_movement(window, camera, FIXED_DT);
			accumulator -= FIXED_DT;
		}

		scene2d.draw_all();
		scene3d.draw_all();

		if(angle >= 360.0f) {
			angle = 0.0f;
		}

		angle += 90.0f * dt; // 90 degrees per second

		window.swap_buffers();
	}
}
