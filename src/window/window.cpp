#include "scarablib/window/window.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mixer.h>

Window::Window(const Window::Config& config)
	: width(config.width), height(config.height), show_debug_info(config.debug_info) {

	// Initialize SDL (video and audio)
	// NOTE: Memory leak happening here
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		throw ScarabError("Failed to init SDL: %s", SDL_GetError());
	}

	// Initialize SDL_mixer
	// NOTE: Reserves couple KBs depending on chunksize
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
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
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
	);

	if(!this->window) {
		Mix_CloseAudio();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to create a SDL window: %s", SDL_GetError());
	}

	// Initialize OpenGL context
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	// SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	// SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	// SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	// SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	this->glContext = SDL_GL_CreateContext(window);
	if(this->glContext == NULL) {
		SDL_DestroyWindow(this->window);
		Mix_CloseAudio();
		SDL_CloseAudio();
		SDL_Quit();
		throw ScarabError("Failed to create OpenGL context: %s", SDL_GetError());
	}

	// Initialize GLEW
	glewExperimental = GL_FALSE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
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
	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // 2D and Skybox support with depth test
	// Culling
	// glEnable(GL_CULL_FACE);


	// SDL Configurations
	SDL_SetWindowResizable(this->window, (SDL_bool)config.resizable);
	if(SDL_GL_SetSwapInterval(config.vsync) < 0) {
		throw ScarabError("Failed to enable vsync: %s", SDL_GetError());
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


void Window::swap_buffers() noexcept {
	// Make operations that need to happen at the end of each frame
	this->frame_events.clear(); // Clear events
	SDL_GL_SwapWindow(this->window);
}

void Window::clear() const noexcept {
	glClearColor(this->clear_color.x, this->clear_color.y, this->clear_color.z, this->clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Window::grab_cursor(const bool grab) const noexcept {
	// BUG:? I HADNT TO DO THIS BEFORE WHY DO I NEED IT NOW?????? WTF????
	// If this isnt't done, the camera will "jump"
	int dummy_x, dummy_y;
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(grab));
	SDL_GetRelativeMouseState(&dummy_x, &dummy_y);
}


float Window::fps() noexcept {
	if(this->delta_time <= 0.0f) {
		return 0.0f;
	}
	return 1.0f / this->delta_time;

	// const float alpha = 0.1f; // Smoothing factor
	// this->smooth_fps = this->smooth_fps * (1.0f - alpha) + (1.0f / this->delta_time) * alpha;
	// return this->smooth_fps;
}

void Window::set_size(const vec2<uint32>& size) noexcept {
	this->width = size.x;
	this->height = size.y;

	// Update window size
	SDL_SetWindowSize(this->window, (int)size.x, (int)size.y);
	glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);

	// Trigger event
	this->frame_events.emplace(Event::WINDOW_RESIZED);
}


bool Window::iskeypressed(const Keycode key) noexcept {
	uint32 scancode = static_cast<uint32>(key);
	if (scancode >= keystate.size()) {
		return false;
	}

	// If is down, change state to "pressed"
	if(this->keystate[scancode] == Keystate::DOWN) {
		this->keystate[scancode] = Keystate::PRESSED;
		return true;
	}

	// If key is pressed, then return as false, since pressed checks one time only
	return false;
}


vec2<int> Window::relative_move() const noexcept {
	vec2<int> mov;
	SDL_GetRelativeMouseState(&mov.x, &mov.y);
	return mov;
}

void Window::frame_capping(const float fps) const noexcept {
	if(fps <= 0.0f) {
		return;
	}

	const float duration = 1.0f / fps;
	const uint64 now     = SDL_GetPerformanceCounter();
	const float elapsed = static_cast<float>(now - this->last_update) / (float)SDL_GetPerformanceFrequency();

	// If not synchronized
	if(elapsed < duration) {
		// Convert to milliseconds, clamp minimum delay to avoid 0ms inaccuracies
		const uint32 delay_ms = static_cast<uint32>((duration - elapsed) * 1000.0f);
		if(delay_ms > 0) {
			SDL_Delay(delay_ms);
		}
	}
}

// private
void Window::calc_dt() noexcept {
	const uint64 now = SDL_GetPerformanceCounter();
	const uint64 elapsed = now - this->last_update;
	this->last_update = now;
	this->delta_time = static_cast<float>(elapsed) / static_cast<float>(SDL_GetPerformanceFrequency());
	// note: elapsed may be multiplied by 1000.0f to get milliseconds

	// Ignore bad frame times (first frame, context switch, GPU stall, etc)
	if(this->delta_time <= 0.0f || this->delta_time >= 0.2f) {
		this->delta_time = 0.0f;
	}
}

// private
void Window::set_viewport(const vec2<uint32>& size) noexcept {
	this->width  = size.x;
	this->height = size.y;
	glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
}

// private
void Window::dispatch_event(const uint32 event) const noexcept {
	// If there is a callback registered for this event
	// (better than a for loop)
	auto it = this->callbacks.find(event);
	if(it != this->callbacks.end()) {
		it->second();
	}
}

void Window::process_events() noexcept {
	// Frame beggining calculations
	this->calc_dt();

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		// Store all events in this frame
		this->frame_events.emplace(event.type);
		// Check if there is a callback
		this->dispatch_event(event.type);

		switch (event.type) {
			// KEYBOARD //
			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				const uint32 scancode = event.key.keysym.scancode;
				// If key is pressed, don't change until is released (avoid changing to down)
				if(event.key.state != 0 && this->keystate[scancode] == Keystate::PRESSED) {
					continue;
				}
				this->keystate[scancode] = static_cast<Keystate>(event.key.state);
				break;
			}

			// MOUSE //
			case SDL_MOUSEBUTTONDOWN: {
				const SDL_MouseButtonEvent button = event.button;
				this->set_buttonstate((Buttoncode)button.button, Buttonstate::DOWN);
				this->clicks = button.clicks;
				this->click_pos = vec2<float>(button.x, button.y);
				break;
			}

			case SDL_MOUSEBUTTONUP: {
				// Reset all
				this->set_buttonstate((Buttoncode)event.button.button, Buttonstate::UP);
				this->clicks = 0;
				this->click_pos = vec3<uint32>(0);
				break;
			}

			case SDL_MOUSEMOTION: {
				const SDL_MouseMotionEvent motion = event.motion;
				this->position  = vec2<float>(motion.x, motion.y);
				this->moved_dir = vec2<float>(motion.xrel, motion.yrel);
				break;
			}

			case SDL_MOUSEWHEEL:
				this->scroll = event.wheel.y > 0;
				break;



			case SDL_WINDOWEVENT: {
				// Store all window events in this frame
				this->frame_events.emplace(event.window.event);

				// Also query for window events
				this->dispatch_event(event.window.event);

				switch (event.window.event) {
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

vec2<int> Window::get_size() noexcept {
	vec2<int> size;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &size.x, &size.y);
	return size;
}
