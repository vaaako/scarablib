#include "scarablib/window/window.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mixer.h>

Window::Window(const Window::Config& config)
	: width(config.width), height(config.height),
	half_width(static_cast<float>(config.width) * 0.5f),
	half_height(static_cast<float>(config.height) * 0.5f), show_debug_info(config.debug_info) {

	// Initialize SDL (video and audio)
	// NOTE: Memory leak happening here
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		throw ScarabError("Failed to init SDL: %s", SDL_GetError());
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		Mix_CloseAudio();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to init SDL_mixer: %s", SDL_GetError());
	}

	// Create SDL window
	this->window = SDL_CreateWindow(
		config.title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		(int)config.width, (int)config.height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (!this->window) {
		Mix_CloseAudio();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to create a SDL window: %s", SDL_GetError());
	}

	// Initialize OpenGL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	this->glContext = SDL_GL_CreateContext(window);
	if (!this->glContext) {
		SDL_DestroyWindow(this->window);
		Mix_CloseAudio();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to create OpenGL context: %s", SDL_GetError());
	}

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		SDL_GL_DeleteContext(this->glContext);
		SDL_DestroyWindow(this->window);
		Mix_CloseAudio();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to init GLEW: %s", glewGetErrorString(err));
	}

	// Configure OpenGL
	glViewport(0, 0, (GLsizei)config.width, (GLsizei)config.height);
	// Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	// 2D and Skybox support
	// glDepthFunc(GL_ALWAYS);
	glDepthFunc(GL_LEQUAL);

	// Cull Face
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);

	// SDL Configurations
	SDL_SetWindowResizable(this->window, (SDL_bool)config.resizable);
	if(SDL_GL_SetSwapInterval(config.vsync) < 0) {
		LOG_ERROR("Failed to enable vsync: %s", SDL_GetError());
	}

	// Debug info
	if(config.debug_info) {
		SDL_version compiled;
		SDL_VERSION(&compiled);

		LOG_INFO("SDL Loaded!");
		LOG_INFO("OpenGL Loaded!");
		LOG_INFO("SDL version: %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
		LOG_INFO("GL Version: %s", glGetString(GL_VERSION));
		LOG_INFO("Vendor: %s", glGetString(GL_VENDOR));
		LOG_INFO("Renderer: %s", glGetString(GL_RENDERER));
		LOG_INFO("Viewport: %dx%d", config.width, config.height);
	}
}

Window::~Window() noexcept {
	if (this->show_debug_info) {
		LOG_INFO("Window %d destroyed", SDL_GetWindowID(this->window));
	}

	// Clean up OpenGL context
	SDL_GL_DeleteContext(this->glContext);
	// Destroy SDL window
	SDL_DestroyWindow(this->window);
	// Close SDL_mixer
	Mix_CloseAudio();
	// Quit SDL
	SDL_Quit();
	// Clean up VAO Manager
	VAOManager::get_instance().cleanup();
	// Clean up Shader Manager
	ShaderManager::get_instance().cleanup();
}

void Window::calc_fps_and_dt() noexcept {
	const uint64 now = SDL_GetPerformanceCounter();
	const uint64 elapsed = now - this->last_update;
	this->last_update = now;
	this->delta_time = static_cast<float>(elapsed) / static_cast<float>(SDL_GetPerformanceFrequency());
	// note: elapsed may be multiplied by 1000.0f to get milliseconds
}

void Window::frame_capping(const uint32 fps) const noexcept {
	const double desired = 1.0 / fps;
	const double actual = this->dt();

	// If not synchronized
	if(actual < desired) {
		// Delay to make it synchronized
		uint32 remaining = static_cast<Uint32>((desired - actual) * 1000);
		SDL_Delay(remaining);
	}
	// If actual >= desired, the frame is already running slower than desired, so no delay is needed
}

void Window::process_events() noexcept {
	// Frame beggining calculations
	this->calc_fps_and_dt();

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
				// Store all window events in this frame
				this->frame_events.emplace(event.window.event);

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

