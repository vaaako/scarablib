#pragma once

#include "scarablib/typedef.hpp"

class UniformBuffer {
	public:
		UniformBuffer() noexcept = default;
		// - `size`: `sizeof(UBO struct)`, std140-aligned.
		// - `binding_point`: Uniform index inside the buffer
		UniformBuffer(const size_t size, const uint32 binding_point);
		~UniformBuffer() noexcept;

		// Delete copy
		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete;

		// UniformBuffer(const UniformBuffer&&) = delete;
		// UniformBuffer& operator=(const UniformBuffer&&) = delete;

		// Update the buffer's data
		void update(const void* data, const size_t size, const uint32 offset = 0) const;

		// Update the buffer's data
		inline void update(const void* data) const {
			this->update(data, this->size, 0);
		};
		
		// void* allocate(const size_t size) const noexcept;

		// Calculates Ring Buffer. Use the returned value as offset on `UniformBuffer::update()`.
		// Sometimes GPU tries to read old data from CPU. This calculates the frame where GPU can write to.
		// - `frameindex`: How many frames was submitted to the GPU.
		//   Calculated as: `framecount % FRAME_IN_FLIGHT`.
		//   Where: `framecount` = Frame index.
		//          `FRAME_IN_FLIGHT` (Recommended: 3) = Max frame where the CPU can submit to the GPU ahead.
		// - `drawindex`: Draw call index within the frame.
		//   (declare a variable equal to 0 and increment it each frame).
		// - `size`: `sizeof(UBO struct)`, std140-aligned.
		// - `maxdraws` (Default: 2048):
		//
		// Use it for:
		//    - Per-draw uniforms: model matrices, material parameters
		//    - Data updated many times per frame
		// Do not use it for:
		//    - Per-frame globals: camera, time, resolution
		//    - Static or rarelyt updated uniforms
		static inline size_t calc_ringbuffer(const uint32 frameindex, const uint32 drawindex, const size_t size, const uint32 maxdraws = 2048) noexcept {
			const size_t framestride = maxdraws * size;

			// FRAME_IN_FLIGHT = 3
			// This is the maximum number of frames the CPU is allowed to submit
			// to the GPU ahead of what the GPU has finished executing
			//
			// framebase   = frameindex * framesride
			// drawoffset  = drawindex * size
			// ring buffer = framebase + drawoffsset
			return frameindex * framestride +
				   drawindex * size;
		}

		// Get the Buffer's ID
		inline uint32 get_id() const noexcept {
			return this->id;
		}

		// Get the size of the Uniform Buffer
		inline size_t get_size() const noexcept {
			return this->size;
		}

		// Get the index of the uniform binding
		inline uint32 get_binding() const noexcept {
			return this->binding;
		}
	
	private:
		size_t size    = 0;
		GLuint id      = 0;
		GLuint binding = 0;
		void* mapped = nullptr;
};
