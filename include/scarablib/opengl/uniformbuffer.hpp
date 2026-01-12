#pragma once

#include "scarablib/typedef.hpp"

class UniformBuffer {
	public:
		UniformBuffer() noexcept = default;
		// - `size`: Buffer's struct size
		// - `binding_point`: Uniform index in the buffer
		// - `dynamic` (Default: false): Enable or disables dynamic update
		UniformBuffer(const size_t size, const uint32 binding_point, bool dynamic = false) noexcept;
		~UniformBuffer() noexcept;

		// Delete copy
		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete;

		// UniformBuffer(const UniformBuffer&&) = delete;
		// UniformBuffer& operator=(const UniformBuffer&&) = delete;

		// Update the buffer's data
		void update(const void* data, const size_t size, const uint32 offset = 0) const;

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
};
