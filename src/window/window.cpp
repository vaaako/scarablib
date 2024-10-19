#include "scarablib/window/window.hpp"
#include "SDL_ttf.h"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_video.h>
#include <SDL_mixer.h>

Window::Window(const WindowConf& config)
	: title(config.title), width(config.width), height(config.height), clear_color(config.clear_color.normalize()), debug_info(config.debug_info) {

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

	if (TTF_Init() != 0) {
		Mix_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to init SDL_ttf: %s", SDL_GetError());
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
		TTF_Quit();
		SDL_Quit();
		throw ScarabError("Failed to create a SDL window: %s", SDL_GetError());
	}

	// Load OpenGL
	this->glContext = SDL_GL_CreateContext(window);

	// Configurations
	SDL_SetWindowResizable(this->window, (SDL_bool)config.resizable);
	if(SDL_GL_SetSwapInterval(true) < 0) {
		LOG_ERROR("Failed to enable vsync: %s", SDL_GetError());
	}

	// Initialize GLEW
	// glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		SDL_GL_DeleteContext(this->glContext);
		SDL_DestroyWindow(this->window);
		Mix_CloseAudio();
		TTF_Quit();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to init GLEW: %s", glewGetErrorString(err));
	}

	// Config opengl
	glViewport(0, 0, (GLsizei)config.width, (GLsizei)config.height);

	// Enable transparency
	// glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Non-premultiplied alpha

	// More config
	glEnable(GL_DEPTH_TEST); // 2D shapes draw order may be opposite because of this
	// glDepthFunc(GL_ALWAYS);

	// Show debug info
	if(debug_info) {
		LOG_INFO("OpengGL Loaded!");
		LOG_INFO("GL Version: %s", glGetString(GL_VERSION));
		LOG_INFO("Vendor: %s", glGetString(GL_VENDOR));
		LOG_INFO("Renderer: %s", glGetString(GL_RENDERER));
		LOG_INFO("Viewport: %dx%d", config.width, config.height);
	}
}

Window::~Window() {
	// Delete handlers
	delete this->keyboard_handler;
	delete this->mouse_handler;

	// Delete window
	if(this->debug_info) {
		LOG_INFO("Window %d destroyed", SDL_GetWindowID(this->window));
	}

	// Close OpenGL
	SDL_GL_DeleteContext(this->glContext);

	// Close all
	SDL_DestroyWindow(this->window);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_CloseAudio();
	SDL_Quit();
}




float Window::fps() {
	uint32 current_time = SDL_GetTicks();

	// Update every second
	if(current_time - start_time >= 1000) {
		// Calc FPS
		this->FPS = static_cast<float>(frame_count) / (static_cast<float>(current_time - start_time) / 1000.0f);

		// Reset
		this->frame_count = 0;
		this->start_time = current_time; // Update timer
	}

	return this->FPS;
}


void Window::process_events() {
	this->last_update = SDL_GetTicks();
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
				this->keyboard_handler->handle_event(event);
				break;

			// Mouse eventsevent
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEWHEEL:
				this->mouse_handler->handle_event(event);
				break;

			/*
			case SDL_WINDOWEVENT: {
				switch (event.window.event) {
					// Focus lost
					case SDL_WINDOWEVENT_FOCUS_LOST:
						break;

					// Mouse leave window
					case SDL_WINDOWEVENT_LEAVE:
						break;

					default:
						break;
				}
				break;
			}
			*/
			default:
				break;
		}
	}
}

