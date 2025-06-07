#pragma once

#include "scarablib/window/events.hpp"
#include "scarablib/input/keyboard.hpp"
#include "scarablib/input/mouse.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/proper/log.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_video.h>

#include <string>
#include <unordered_set>

// Main object of the library, used for making and managing a window and events
class Window {
	public:
		// Events related to the window

		struct Config {
			// Required values
			uint32 width;
			uint32 height;
			std::string title; // char* in c++ goes grrr

			// Optional
			bool vsync = true;
			bool resizable = false;
			bool debug_info = false;
		};

		Window(const Window::Config& windowconf);
		~Window() noexcept;

		// Handle any events occurring in the window.
		// Also calculates FPS and Delta time
		void process_events() noexcept;

		// Call the callback function for each handled SDL event, passing SDL_Event as a parameter.
		// This is generally used for ImGui process events.
		// Example: `window.raw_events([](SDL_Event event) { ImGui_ImplSDL2_ProcessEvent(&event); });`
		template<typename T>
		void raw_events(T&& callback) noexcept {
			SDL_Event event;
			while(SDL_PollEvent(&event)) {
				std::forward<T>(callback)(event);
			}
		}

		// WINDOW PROCESS //

		// True while the window is open
		inline bool is_open() const noexcept {
			return this->window_open;
		}

		// Swap the front and back buffers at the end of each frame.
		// Also clears events buffer.
		// This should be called at the end of each frame
		// to display the newly rendered frame to the screen
		inline void swap_buffers() noexcept {
			// Make operations that need to happen at the end of each frame
			this->frame_events.clear(); // Clear events
			SDL_GL_SwapWindow(this->window);
		}

		// Clear the screen by setting the background color and clearing the buffer.
		// This should be called at the beginning of each frame to reset the drawing surface.
		// It clears both the color and depth buffers.
		inline void clear() const noexcept {
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}


		// GETTERS //

		// Return a pointer to the SDL_Window object.
		// This can be used to directly manipulate or access SDL window properties.
		inline SDL_Window* get_reference() const noexcept {
			return this->window;
		}

		// Return the OpenGL context object
		inline SDL_GLContext get_gl_context() const noexcept {
			return this->glContext;
		}

		// Return the width of the window in pixels.
		inline uint32 get_width() const noexcept {
			return this->width;
		}

		// Return the height of the window in pixels.
		inline uint32 get_height() const noexcept {
			return this->height;
		}

		// Return the half width of the window in pixels.
		inline float get_half_width() const noexcept {
			return this->half_width;
		}

		// Return the half height of the window in pixels.
		inline float get_half_height() const noexcept {
			return this->half_height;
		}

		// Return a pointer to the keyboard handler object.
		// This allows access to input handling for the keyboard.
		inline KeyboardHandler& keyboard() noexcept {
			return this->keyboard_handler;
		}

		// Return a pointer to the mouse handler object.
		// This allows access to input handling for the mouse.
		inline MouseHandler& mouse() noexcept {
			return this->mouse_handler;
		}


		// SETTERS

		// Change the window's title to the provided value.
		inline void set_title(const std::string& title) noexcept {
			SDL_SetWindowTitle(this->window, title.c_str());
		}

		// Set the window's clear color (background color) using the provided Color object.
		inline void set_clear_color(const Color& clear_color) noexcept {
			this->clear_color = clear_color.normalize();
		}

		// Enable or disable the window's resizable property.
		inline void set_resizable(const bool enable) noexcept  {
			SDL_SetWindowResizable(this->window, static_cast<SDL_bool>(enable));
		}

		// Enable or disable the window border
		inline void set_border(const bool enable) noexcept {
			SDL_SetWindowBordered(this->window, static_cast<SDL_bool>(enable));
		}

		// Enable or disable vertical sync (VSync).
		// VSync synchronizes the frame rate with monitor's refresh rate.
		// Disable it for potentially higher frame rates.
		inline void set_vsync(const bool state) const noexcept {
			if(SDL_GL_SetSwapInterval(state) < 0) {
				LOG_ERROR("Failed to enable vsync: %s", SDL_GetError());
			}
		}

		// Sets the polygon drawing mode for both front and back faces.
		// This makes 3D render more optimized but 2D rendering will not work
		inline void set_cullface(const bool state) const noexcept {
			if(state) {
				glEnable(GL_CULL_FACE);
			} else {
				glDisable(GL_CULL_FACE);
			}
		}

		// Sets the depth test to always pass.
		// If false, sets to LEQUAL (default).
		// This prevents 2D rendering from blending with 3D meshes.
		// But 3D rendering will not work properly
		inline void set_depthtest_always(const bool state) const noexcept {
			if(state) {
				glDepthFunc(GL_ALWAYS);
			} else {
				glDepthFunc(GL_LEQUAL);
			}
		}

		// Disabled cull face and sets depth test to always pass.
		// This makes 2D rendering more optimized accurate but 3D rendering will not work properly
		inline void set_2dmode(const bool state) const noexcept {
			if(state) {
				glDisable(GL_CULL_FACE);
				glDepthFunc(GL_ALWAYS);
			} else {
				glEnable(GL_CULL_FACE);
				glDepthFunc(GL_LEQUAL);
			}
		}

		// Set the window's size using the provided width and height values.
		// This also adjusts the OpenGL viewport to match the new window size.
		void set_size(const vec2<uint32>& size) noexcept;


		// CHECKERS //

		// Close the window by setting the `window_open` flag to false.
		// This flag can be checked in the main loop to terminate or pause rendering.
		inline void close() noexcept {
			this->window_open = false;
		}

		// Check if a given event is present in the current frame's event buffer.
		// If the event exists in the buffer, it is considered "activated".
		inline bool has_event(const Event event) const noexcept {
			return this->frame_events.find(event) != this->frame_events.end();
		}

		// Run a callback everytime a given event is present in the current frame's event buffer
		template<typename T>
		inline void on_event(const Event event, T&& callback) const noexcept {
			if(this->has_event(event)) {
				std::forward<T>(callback)();
			}
		}

		// Check if the provided X or Y coordinates are outside the window's bounds.
		inline bool is_out_of_bounds(const uint32 x, const uint32 y) const noexcept {
			return ((int)x < 0 || x > this->width) ||
				   ((int)y < 0 || y > this->height);
		}


		// FUNCTIONALITIES //

		// Locks the cursor inside the window.
		// This prevents the cursor from leaving the window when on focus.
		inline void jail_cursor(const bool status) const noexcept {
			SDL_SetWindowGrab(this->window, static_cast<SDL_bool>(status));
		}

		// Hides and grabs the cursor inside the window
		inline void grab_cursor(const bool grab) const noexcept {
			SDL_SetRelativeMouseMode(static_cast<SDL_bool>(grab));

			// BUG:? I HADNT TO DO THIS BEFORE WHY DO I NEED IT NOW?????? WTF????
			// If this isnt't done, the camera will "jump"
			if(grab) {
				int dummy_x, dummy_y;
				SDL_GetRelativeMouseState(&dummy_x, &dummy_y);
			}
		}

		// Hide the cursor when it is inside the window.
		inline void hide_cursor(const bool hide) const noexcept {
			SDL_ShowCursor(!hide);
		}

		// TIMER //

		// Window FPS per second
		inline float fps() const noexcept {
			return 1.0f / this->delta_time;
		}

		// Get the time elapsed between the current and last frame, in seconds.
		inline float dt() const noexcept {
			return this->delta_time;
		}

		// Stabilize the FPS and tries to limit the frame rate.
		// Frame capping will not work if VSync is enabled
		void frame_capping(const float fps) const noexcept;

		// Optional `time` parameter adjusts the time scale (default: 1000.0 for seconds).
		// Get the number of milliseconds that have passed since the SDL library was initialized.
		// Note: The value wraps after approximately 49 days of continuous program execution.
		inline uint32 timenow() const noexcept {
			return SDL_GetTicks();
		}



		// STATIC //

		// Get the current width of the window (static method).
		// This retrieves the window size for the active OpenGL context.
		static inline int static_width() noexcept {
			int width;
			SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, NULL);
			return width;
		}

		// Get the current height of the window (static method).
		// This retrieves the window size for the active OpenGL context.
		static inline int static_height() noexcept {
			int height;
			SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), NULL, &height);
			return height;
		}

	private:
		// Window information
		uint32 width;
		uint32 height;
		float half_width        = 0;
		float half_height       = 0;
		bool show_debug_info    = false;
		vec4<float> clear_color = vec4<float>(0.12f, 0.12f, 0.12f, 1.0f);
		bool window_open        = true;

		// Buffer to store all events to be processed each frame
		std::unordered_set<uint32> frame_events; // unordered_set is faster for look-up

		// SDL2
		SDL_Window* window;
		SDL_GLContext glContext;

		// FPS and DT
		uint32 last_time   = SDL_GetTicks();
		float delta_time   = 1.0f;
		uint64 last_update = 0; // Updated on swap_buffers (frame end)

		// KEYS
		KeyboardHandler keyboard_handler = KeyboardHandler();
		MouseHandler mouse_handler = MouseHandler();

		// Called at the beggining of the frame.
		void calc_fps_and_dt() noexcept;

		// Set the viewport to the provided size.
		// This won't change the window's size
		inline void set_viewport(const vec2<uint32>& size) noexcept {
			this->width  = size.x;
			this->height = size.y;

			this->half_width  = static_cast<float>(size.x) * 0.5f;
			this->half_height = static_cast<float>(size.y) * 0.5f;
			glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
		}
};
