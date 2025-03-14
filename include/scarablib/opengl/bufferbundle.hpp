#pragma once

#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/opengl/vao_manager.hpp"

// A class that bundles the VAO, VBO and EBO buffers.
// It has methods for easy create of them
class BufferBundle {
	public:
		// Easy create and store manager for VAO, VBO and EBO
		BufferBundle() noexcept = default;
		~BufferBundle() noexcept;

		// Enable the VAO
		inline void bind_vao() const noexcept {
			glBindVertexArray(this->vao_id);
		}

		// Disable the VAO
		inline void unbind_vao() const noexcept {
			glBindVertexArray(0);
		}

		inline uint32 get_vao_id() const noexcept {
			return this->vao_id;
		}

		// Creates the VAO and you are responsible for managing.
		// Indices is an optional parameter, you can pass an empty vector.
		void make_vao(const std::vector<Vertex>& vertices, std::vector<uint32>& indices) noexcept;

		// Uses the VAO Manager to create the VAO.
		// Vertices may be of type vec3<float> or Vertex.
		// The VBO and EBO are managed by the VAO Manager.
		// Indices is an optional parameter, you can pass an empty vector.
		template <typename T>
		inline void make_vao_with_manager(const std::vector<T>& vertices, const std::vector<uint32>& indices) noexcept {
			static_assert(std::is_same<T, vec3<float>>::value || std::is_same<T, Vertex>::value,
					"Only vec3<float> and Vertex types for vertices are accepted");

			this->vao_hash = VAOManager::get_instance().compute_hash(vertices, indices);
			this->vao_id = VAOManager::get_instance().get_or_make_vao(
				vertices, indices
			);
		}

		template <typename T>
		inline void make_vao_with_manager(const std::vector<std::vector<vec3<float>>>& data, const std::vector<uint32>& indices) noexcept {
			this->vao_hash = VAOManager::get_instance().compute_hash(data.at(0), indices);
			this->vao_id = VAOManager::get_instance().get_or_make_vao(
				data, indices
			);
		}

	private:
		VBO* vbo = nullptr;
		EBO* ebo = nullptr;

		size_t vao_hash = 0; // Only used for VAOManager
		uint32 vao_id   = 0;
};
