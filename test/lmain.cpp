#include "scarablib/legacyopengl/lcamera2d.hpp"
#include "scarablib/legacyopengl/lcamera3d.hpp"
#include "scarablib/legacyopengl/lshapes.hpp"
#include "scarablib/window/window.hpp"

int main() {
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = "FPS: 0",
		.vsync = true,
		.resizable = true,
		.debug_info = true
	});

	LCamera2D cam2d = LCamera2D(window);
	LCamera3D cam3d = LCamera3D(window);

	float angle = 0.0f;
	while(window.is_open()) {
		window.clear();
		window.process_events();

		if(window.has_event(Event::WINDOW_QUIT)) {
			window.close();
		}

		cam2d.begin();
			LShapes::set_color(Colors::RED);
			LShapes::draw_rectangle({ 100.0f, 100.0f }, { 100.0f, 100.0f });
		cam2d.end();

		cam3d.begin();
			LShapes::draw_cube({ 0.0f, 0.0f, -5.0f }, 1.0f, angle);
		cam3d.end();

		if(angle >= 360.0f) {
			angle = 0.0f;
		}
		angle += 1.0f;

		window.swap_buffers();
	}
}
