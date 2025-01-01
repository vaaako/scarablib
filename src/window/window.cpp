#include "scarablib/window/window.hpp"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include "scarablib/proper/error.hpp"
#include "scarablib/utils/math.hpp"
#include <SDL2/SDL_mixer.h>

Window::Window(const WindowConf& config) : conf(config), half_width(config.width / 2), half_height(config.height / 2) {
	// Init SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw ScarabError("Failed to init SDL: %s", SDL_GetError());
	}

	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		throw ScarabError("Failed to init audio: %s", SDL_GetError());
	}

	// Init SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		Mix_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to init SDL_mixer: %s", SDL_GetError());
	}

	// Make window
	this->window = SDL_CreateWindow(
		config.title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		(int)config.width, (int)config.height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if(!this->window) {
		Mix_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to create a SDL window: %s", SDL_GetError());
	}

	// Load OpenGL
	this->glContext = SDL_GL_CreateContext(window);

	// Configurations
	SDL_SetWindowResizable(this->window, (SDL_bool)config.resizable);
	if(SDL_GL_SetSwapInterval(config.vsync) < 0) {
		LOG_ERROR("Failed to enable vsync: %s", SDL_GetError());
	}

	// Initialize GLEW
	// glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		SDL_GL_DeleteContext(this->glContext);
		SDL_DestroyWindow(this->window);
		Mix_CloseAudio();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to init GLEW: %s", glewGetErrorString(err));
	}

	// Config opengl
	glViewport(0, 0, (GLsizei)config.width, (GLsizei)config.height);

	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);



	// Show debug info
	if(config.debug_info) {
		LOG_INFO("OpengGL Loaded!");
		LOG_INFO("GL Version: %s", glGetString(GL_VERSION));
		LOG_INFO("Vendor: %s", glGetString(GL_VENDOR));
		LOG_INFO("Renderer: %s", glGetString(GL_RENDERER));
		LOG_INFO("Viewport: %dx%d", config.width, config.height);
	}
}

Window::~Window() {
	// Delete handlers
	// delete this->keyboard_handler;
	// delete this->mouse_handler;

	// Delete window
	if(this->conf.debug_info) {
		LOG_INFO("Window %d destroyed", SDL_GetWindowID(this->window));
	}

	// Close OpenGL
	SDL_GL_DeleteContext(this->glContext);

	// Close all
	SDL_DestroyWindow(this->window);
	Mix_CloseAudio();
	SDL_CloseAudio();
	SDL_Quit();
}




double Window::fps() {
	const uint32 current = SDL_GetTicks();
	double elapsed = current - this->start_time;

	// Update every second
	if(elapsed >= 1000) {
		// Avoid division by zero
		if(elapsed == 0.0f) {
			elapsed = 0.01f;
		}

		// Calculate FPS
		this->FPS = this->frame_count / (elapsed / 1000.0);

		// Reset
		this->frame_count = 0;
		this->start_time = current; // Update timer
	}

	return this->FPS;
}

double Window::dt() const {
	const uint32 current = SDL_GetTicks();
	const uint32 elapsed = current - this->last_update;
	// if(this->frame_count < desired_fps) {
	// 	return;
	// }

	// Handle wrap-around (around 49 days)
	// if(elapsed > 0x7FFFFFFF) {
	// 	elapsed = 0.01f; // Reset or handle appropriately
	// 	// Make 0.01 to avoid division by 0
	// }

	// Convert milliseconds to seconds
	return static_cast<double>(elapsed) / 1000.0;
}

#include <iostream>
void Window::frame_capping(const uint32 fps) const {
	const double desired = 1.0 / fps; // ~0.0133 seconds per frame
	const double actual = this->dt();

	// If not synchronized
	if(actual < desired) {
		// Delay to make it synchronized
		uint32 remaining = static_cast<Uint32>((desired - actual) * 1000);
		SDL_Delay(remaining);
		// Simulate 60 FPS if remaining is less than 10ms
		// (remaining > 10) ? 5 : remaining
	}
	// If actual >= desired, the frame is already running slower than desired, so no delay is needed
}


void Window::process_events() {
	this->frame_count++;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		// Store all events in this frame
		this->frame_events.emplace(event.type);

		// Some events need to be handled
		switch (event.type) {
			// Keyboard events
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				this->keyboard_handler.handle_event(event);
				break;

			// Mouse eventsevent
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEWHEEL:
				this->mouse_handler.handle_event(event);
				break;

			case SDL_WINDOWEVENT: {
				switch (event.window.event) {
					// // Focus lost
					// case SDL_WINDOWEVENT_FOCUS_LOST:
					// 	break;
					//
					// // Mouse leave window
					// case SDL_WINDOWEVENT_LEAVE:
					// 	break;

					case SDL_WINDOWEVENT_RESIZED:
						this->set_viewport(vec2<uint32>(event.window.data1, event.window.data2));
						break;


					default:
						break;
				}
				break;
			}
			default:
				break;
		}
	}
}

