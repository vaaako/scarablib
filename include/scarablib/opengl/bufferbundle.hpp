#pragma once

#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/opengl/vao_manager.hpp"

// A class that bundles the VAO, VBO and EBO buffers.
// It has methods for easy create of them
class BufferBundle {
	public:
		VBO* vbo = nullptr;
		EBO* ebo = nullptr;

		// Easy create and store manager for VAO, VBO and EBO
		BufferBundle() noexcept = default;
		~BufferBundle() noexcept;

		// Enable the VAO
		inline void bind_vao() const noexcept {
			glBindVertexArray(this->vao_id);
		}

		// Automatically creates a EBO and stores it.
		// WARNING: Do not use this method if you are using the VAO Manager
		inline void make_ebo(const std::vector<uint32> indices) noexcept {
			this->ebo = new EBO(indices);
		}

		// Initializes the VBO.
		// This method will simple make a new VBO and store it.
		// You still need to create it manually
		// WARNING: Do not use this method if you are using the VAO Manager
		void make_vbo() noexcept {
			this->vbo = new VBO();
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
		template <typename T>
		void make_vao(const std::vector<Vertex>& vertices, std::vector<T>& indices) noexcept;

		// Uses the VAO Manager to create the VAO.
		// Vertices may be of type vec3<float> or Vertex.
		// The VBO and EBO are managed by the VAO Manager.
		// Indices is an optional parameter, you can pass an empty vector.
		// WARNING: If you are using this method do not build the VBO and EBO yourself or use make_vao
		template <typename T, typename U>
		inline void make_vao_with_manager(const std::vector<T>& vertices, const std::vector<U>& indices) noexcept {
			// Asserts are done inside VAOManager methods

			this->vao_hash = VAOManager::get_instance().compute_hash(vertices, indices);
			this->vao_id = VAOManager::get_instance().get_or_make_vao(
				vertices, indices
			);
		}

	private:
		size_t vao_hash = 0; // Only used for VAOManager
		uint32 vao_id   = 0;
};

template <typename T>
void BufferBundle::make_vao(const std::vector<Vertex>& vertices, std::vector<T>& indices) noexcept {
	static_assert(
		std::is_same_v<T, uint32> ||
		std::is_same_v<T, uint16> ||
		std::is_same_v<T, uint8>,
		"Only uint32, uint16 and uint8 types for indices are accepted");

	bool has_indices = !indices.empty();

	glGenVertexArrays(1, &this->vao_id);
	glBindVertexArray(this->vao_id);

	if(has_indices) {
		this->ebo = new EBO(indices);
	}

	// Make VBO
	this->vbo =  new VBO();
	this->vbo->bind();
	this->vbo->alloc_data(vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Link Position and texuv
	this->vbo->link_attrib(0, 3, sizeof(Vertex), 0);
	this->vbo->link_attrib(1, 2, sizeof(Vertex), offsetof(Vertex, texuv));

	// Unbind all
	glBindVertexArray(0);
	this->vbo->unbind();

	if(has_indices) {
		this->ebo->unbind();
	}
}
