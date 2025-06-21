#pragma once

#include "scarablib/proper/log.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/window/buttoncode.hpp"
#include "scarablib/window/events.hpp"
#include "scarablib/window/keycode.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

// Main object of the library, used for making and managing a window and events
class Window {
	public:
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


		// WINDOW PROCESS //

		// True while the window is open
		inline bool is_open() const noexcept {
			return this->window_open;
		}

		// Swap the front and back buffers at the end of each frame.
		// Also clears events buffer.
		// This should be called at the end of each frame
		// to display the newly rendered frame to the screen
		void swap_buffers() noexcept;

		// Clear the screen by setting the background color and clearing the buffer.
		// This should be called at the beginning of each frame to reset the drawing surface.
		// It clears both the color and depth buffers.
		void clear() const noexcept;


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

		// Return all events that occurred in the current frame.
		// Convert it to SDL_Event enum or Event enum
		inline std::unordered_set<uint32> get_events() noexcept {
			return this->frame_events;
		}


		// SETTERS

		// Change the window's title to the provided value.
		inline void set_title(const std::string_view& title) noexcept {
			SDL_SetWindowTitle(this->window, title.data());
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

		// Hides the back face and sets the polygon drawing mode for front face.
		// This makes 3D render more optimized but 2D rendering will not work
		inline void set_cullface(const bool state) const noexcept {
			(state) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		}

		// Sets the polygon drawing mode for both front and back faces.
		// Cullface must be enable to work.
		// This makes 3D render more optimized but 2D rendering will not work
		// inline void set_cullface_back(const bool state) const noexcept {
		// 	glCullFace(GL_FRONT + state); // true => 1028 + 1 = > 1029 (GL_BACK)
		// }

		// Sets the depth test to always pass.
		// If false, sets to LEQUAL (default).
		// This prevents 2D rendering from blending with 3D meshes.
		// But 3D rendering will not work properly
		inline void set_depthtest_always(const bool state) const noexcept {
			glDepthFunc(GL_LEQUAL + (state * 4)); // true => 1 * 4 => 515 + 4 => 519 (GL_ALWAYS)
		}

		// Disabled cull face and sets depth test to always pass.
		// This makes 2D rendering more optimized accurate but 3D rendering will not work properly
		inline void set_2dmode(const bool state) const noexcept {
			this->set_cullface(state);
			this->set_depthtest_always(state);
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
		inline void on_event(const Event event, const std::function<void()>& callback) const noexcept {
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
		void grab_cursor(const bool grab) const noexcept;

		// Hide the cursor when it is inside the window.
		inline void hide_cursor(const bool hide) const noexcept {
			SDL_ShowCursor(!hide);
		}

		// TIMER //

		// Window FPS per second
		float fps() noexcept;

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


		// INPUT HANDLERS //

		// General purpose state check for Keyboard and Mouse
		template<typename StateEnum, typename CodeEnum>
		inline bool is_state(const std::vector<StateEnum>& states, const CodeEnum code, const StateEnum expected) const noexcept {
			return states[static_cast<uint32>(code)] == expected;
		}

		// Returns true while the key is down
		inline bool iskeydown(const Keycode key) const noexcept {
			return this->is_state(this->keystate, key, Keystate::DOWN);
		}

		// Returns true if a key is up
		inline bool iskeyup(const Keycode key) const noexcept {
			return this->is_state(this->keystate, key, Keystate::UP);
		}

		// Return true if a key is pressed.
		// Like iskeydown, but returns only once
		bool iskeypressed(const Keycode key) noexcept;

		// Returns the state of a key
		inline Keystate get_keystate(const Keycode key) const noexcept {
			return this->keystate[static_cast<uint32>(key)];
		}

		// Changes the state of a key
		inline void set_keystate(const Keycode key, const Keystate state) noexcept {
			this->keystate[static_cast<uint32>(key)] = state;
		}

		// MOUSE

		// Returns cursor click position
		inline vec2<uint32> get_click_pos() const noexcept {
			return this->click_pos;
		}

		// Returns cursor motion/position
		inline vec2<uint32> get_position() const noexcept {
			return this->position;
		}

		// Returns mouse moved direction (e.g. -1, 1 = Left Down)
		inline vec2<int16> get_moved_dir() const noexcept {
			return this->moved_dir;
		}

		// Returns mouse movent since last call
		vec2<int> relative_move() const noexcept;

		// Returns true if a button was clicked
		inline bool isbtndown(const Buttoncode button) const noexcept {
			return this->is_state(this->buttonstate, button, Buttonstate::DOWN);
		}

		// Checks if a button is released
		inline bool isbtnup(const Buttonstate button) const noexcept {
			return this->is_state(this->buttonstate, button, Buttonstate::UP);
		}

		// Changes the state of a button
		inline void set_buttonstate(const Buttoncode button, const Buttonstate state) noexcept {
			this->buttonstate[static_cast<uint32>(button)] = state;
		}

		// Checks the state of a button
		inline Buttonstate get_keystate(const Buttoncode button) const {
			return this->buttonstate[static_cast<uint32>(button)];
		}

		// Changes cursor position in window
		inline void set_cursor_position(const uint32 x, const uint32 y) noexcept {
			SDL_WarpMouseInWindow(this->window, (int)x, (int)y);
		}


		// STATIC //

		// Get the current size of the window
		static vec2<int> get_size() noexcept;

	private:
		// Window information
		uint32 width;
		uint32 height;
		bool show_debug_info    = false;
		vec4<float> clear_color = vec4<float>(0.12f, 0.12f, 0.12f, 1.0f);
		bool window_open        = true;

		// Buffer to store all events to be processed each frame
		std::unordered_set<uint32> frame_events;

		// SDL2
		SDL_Window* window;
		SDL_GLContext glContext;

		// FPS and DT
		uint32 last_time   = SDL_GetTicks();
		float delta_time   = 1.0f;
		uint64 last_update = 0; // Updated on swap_buffers (frame end)

		// INPUT HANDLERS
		// Keyboard events in this frame
		// Init vector with the size of SDL scancodes
		std::vector<Keystate> keystate
			= std::vector<Keystate>((uint32)Keycode::NUM_SCANCODES, Keystate::UP);

		// Store the state of all mouse buttons (LMB, MMB, RMB, SIDE1, SIDE2)
		std::vector<Buttonstate> buttonstate
			= std::vector<Buttonstate>(6, Buttonstate::UP);

		uint32 clicks = 0;      // Clicks made
		int scroll = 0;         // Scroll direction and amount
		// Store all actions in frame to handle later
		vec2<uint32> click_pos; // Click position  (X: 0 -> width / Y: 0 -> height)
		vec2<uint32> position;  // Motion position (X: 0 -> width / Y: 0 -> height)
		vec2<int16> moved_dir;  // Normaalized moved_dir
		// xrel yrel

		// Called at the beggining of the frame.
		void calc_dt() noexcept;

		// Set the viewport to the provided size.
		// This won't change the window's size
		void set_viewport(const vec2<uint32>& size) noexcept;
};
