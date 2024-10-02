#pragma once

#include "scarablib/proper/log.hpp"
#include "scarablib/proper/vector/vec2.hpp"
#include "scarablib/window/window_conf.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/input/keyboard.hpp"
#include "scarablib/input/mouse.hpp"
#include "scarablib/window/events.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <SDL_audio.h>
#include <SDL_ttf.h>

#include <GL/glew.h>

#include <string>
#include <unordered_set>

// Main object of the library, used for making and managing a window and events
class Window {
	public:
		Window(const WindowConf& windowconf);
		~Window();

		// Handle any events or inputs (e.g., keyboard, mouse) that occurred during the frame.
		// This function should be called once at the beginning of each frame to process input events.
		void process_events();


		// WINDOW PROCESS //

		// True while the window is open
		inline bool is_open() const {
			return this->window_open;
		}

		// Swap the front and back buffers at the end of each frame.
		// This should be called at the end of each frame
		// to display the newly rendered frame to the screen
		inline void swap_buffers() {
			#ifdef SCARAB_DEBUG_EVENT_COUNT
				if(frame_events.size() > 0) {
					std::printf("Events in this frame: %zu\n", frame_events.size());
					if(frame_events.size() > 5) {
						for(uint32 event : frame_events) {
							std::printf("+ event: %u\n", event);
						}
					
					}
				}
			#endif

			this->frame_events.clear(); // Clear events
			SDL_GL_SwapWindow(this->window);
		}

		// Clear the screen by setting the background color and clearing the buffer.
		// This should be called at the beginning of each frame to reset the drawing surface.
		// It clears both the color and depth buffers.
		inline void clear() const {
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}


		// GETTERS //

		// Return a pointer to the SDL_Window object.
		// This can be used to directly manipulate or access SDL window properties.
		inline SDL_Window* get_reference() const {
			return this->window;
		}

		// Return the current title of the window as a string.
		inline std::string get_title() const {
			return this->title;
		}

		// Return the width of the window in pixels.
		inline uint32 get_width() const {
			return this->width;
		}

		// Return the height of the window in pixels.
		inline uint32 get_height() const {
			return this->height;
		}

		// Return a pointer to the keyboard handler object.
		// This allows access to input handling for the keyboard.
		inline Keyboard* keyboard() const {
			return this->keyboard_handler;
		}

		// Return a pointer to the mouse handler object.
		// This allows access to input handling for the mouse.
		inline Mouse* mouse() const {
			return this->mouse_handler;
		}


		// SETTERS

		// Change the window's title to the provided value.
		inline void set_title(char* title) {
			this->title = title; // char*
			SDL_SetWindowTitle(this->window, title);
		}

		// Set the window's clear color (background color) using the provided Color object.
		inline void set_clear_color(const Color& clear_color) {
			this->clear_color = clear_color.normalize();
		}

		// Enable or disable the window's resizable property.
		inline void set_resizable(const bool enable) {
			SDL_SetWindowResizable(this->window, static_cast<SDL_bool>(enable));
		}

		// Enable or disable vertical sync (VSync).
		// VSync synchronizes the frame rate with monitor's refresh rate.
		// Disable it for potentially higher frame rates.
		inline void set_vsync(const bool enable) {
			if(SDL_GL_SetSwapInterval(enable) < 0) {
				LOG_ERROR("Failed to enable VSync: %s", SDL_GetError());
			}
		}

		// Set the window's size using the provided width and height values.
		// This also adjusts the OpenGL viewport to match the new window size.
		inline void set_size(const vec2<uint32>& size) {
			this->width = size.x;
			this->height = size.y;
			SDL_SetWindowSize(this->window, (int)size.x, (int)size.y);
			glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
		}


		// CHECKERS //

		// Close the window by setting the `window_open` flag to false.
		// This flag can be checked in the main loop to terminate or pause rendering.
		inline void close() {
			this->window_open = false;
		}

		// Check if a given event is present in the current frame's event buffer.
		// If the event exists in the buffer, it is considered "activated".
		inline bool on_event(Event event) const {
			return this->frame_events.find(static_cast<uint32>(event)) != this->frame_events.end();
		}

		// Check if the provided X or Y coordinates are outside the window's bounds.
		inline bool out_of_bounds(const int x, const int y) const {
			return (x < 0 || x > (int)this->width) ||
				   (y < 0 || y > (int)this->height);
		}


		// FUNCTIONALITIES //

		// Lock the cursor to the center of the window.
		// This prevents the cursor from leaving the window when the game is in focus.
		inline void grab_cursor(const bool grab) const {
			SDL_SetRelativeMouseMode(static_cast<SDL_bool>(grab));
		}

		// Hide the cursor when it is inside the window.
		inline void hide_cursor(const bool hide) const {
			SDL_ShowCursor(!hide);
		}


		// TIMER //

		// Automatically calculates FPS.
		float fps();

		// Get the number of milliseconds that have passed since the SDL library was initialized.
		// Note: The value wraps after approximately 49 days of continuous program execution.
		inline uint32 time() const {
			return SDL_GetTicks();
		}

		// Get the time elapsed between the current and last frame, in seconds.
		// Optional `time` parameter adjusts the time scale (default: 1000.0 for seconds).
		inline double dt(const float time = 1000.0f) const {
			// CURRENT - LAST / to seconds
			return static_cast<double>(SDL_GetTicks() - this->last_update) / time;
		}


		// STATIC //

		// Get the current width of the window (static method).
		// This retrieves the window size for the active OpenGL context.
		static inline int static_width() {
			int width;
			SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, NULL);
			return width;
		}

		// Get the current height of the window (static method).
		// This retrieves the window size for the active OpenGL context.
		static inline int static_height() {
			int height;
			SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), NULL, &height);
			return height;
		}

	private:
		// Window information
		char* title;
		uint32 width;
		uint32 height;
		vec4<float> clear_color;
		bool debug_info = false;
		bool window_open = true;

		// Buffer to store all events to be processed each frame
		std::unordered_set<uint32> frame_events; // unordered_set is faster for look-up

		// SDL2
		SDL_Window* window;
		SDL_GLContext glContext;

		// FPS
		uint32 start_time = SDL_GetTicks();
		uint32 last_update = 0;
		uint32 frame_count = 0;
		float FPS = 0.0f;

		// KEYS
		// NOTE -- Since window has a get for these objects, it need to be pointers, so i can pass as reference to other methods
		Keyboard* keyboard_handler = new Keyboard();
		Mouse* mouse_handler = new Mouse(); // To not have the same name as the "mouse" function
};
