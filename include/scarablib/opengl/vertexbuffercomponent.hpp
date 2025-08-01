#pragma once

#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"

// A class that bundles the VAO, VBO and EBO buffers.
// Also stores information necessary for rendering like indices size and indices type.
// With utilitary methods for creating the Vertex Buffer
class VertexBufferComponent {
	public:
		// Must be initialized with some constructor
		VAO* vao = nullptr;
		// Optional, not always required
		VBO* vbo = nullptr;
		// Optional, not always required
		EBO* ebo = nullptr;

		VertexBufferComponent() noexcept = default;
		~VertexBufferComponent() noexcept;

		// Returns unique hash used in VAOManager to get this VAO
		inline constexpr size_t get_hash() const noexcept {
			return this->hash;
		}

		// Returns the length of this VAO.
		// This is used for rendering
		// For 3D shapes this is the length of indices.
		// For 2D shapes this is the length of vertices.
		inline constexpr int get_length() const noexcept {
			return this->length;
		}

		// Returns the type of indices.
		// This is used for 3D rendering
		inline constexpr GLenum get_indices_type() const noexcept {
			return this->indices_type;
		}

		// Configurates additional attributes for data.
		// This will only be used if data is a vector of Vertex (or child of).
		// - `count`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `type`: (Default: GL_FLOAT) Type of data (e.g., GL_FLOAT).
		// - `normalized`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
		void add_attribute(const uint32 count, const GLenum type = GL_FLOAT, const bool normalized = false);

		// Automatically computes Vertex Buffer and stores in VAOManager
		template <typename T, typename U>
		void make_vao(const std::vector<T>& vertices, const std::vector<U>& indices);

	private:
		struct VertexAttribute {
			// How many components per attribute.
			// Example: 2 for a vec2
			const uint32 component_count;
			// Type of data
			// Example: GL_FLOAT
			const GLenum type;
			// Whether to normalize the data
			const bool normalized;
			// Offset of the attribute
			const size_t offset;
		};

		std::vector<VertexAttribute> attributes;
		// Current offset of attributes
		size_t stride = sizeof(vec3<float>); // position is always presented
		size_t hash;
		// For rendering
		// in 2D this is vertices size
		int length = 0;
		// For rendering
		// not used for 2D shapes
		GLenum indices_type = GL_UNSIGNED_INT;

		inline void cleanup_attributes() noexcept {
			// Clear attributes
			this->attributes.clear();
			this->attributes.shrink_to_fit(); // Actually releases the allocated memory
			this->stride = sizeof(vec3<float>);
		}
};

template <typename T, typename U>
void VertexBufferComponent::make_vao(const std::vector<T>& vertices, const std::vector<U>& indices) {
	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Making VAO with %zu vertices and %zu indices", vertices.size(), indices.size());
	#endif

	static_assert(std::is_same_v<T, vec3<float>> || std::is_base_of_v<Vertex, T>,
			"Only vec3<float> and Vertex types for vertices are accepted");

	static_assert(
		std::is_same_v<U, uint32> ||
		std::is_same_v<U, uint16> ||
		std::is_same_v<U, uint8>,
		"Only uint32, uint16 and uint8 types for indices are accepted");

	if(vertices.empty()) {
		throw ScarabError("Vertices vector is empty");
	}

	if(!indices.empty()) {
		this->length = indices.size();
		this->indices_type = (std::is_same_v<U, uint32>) ? GL_UNSIGNED_INT :
			(std::is_same_v<U, uint16>) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;
	} else {
		this->length = vertices.size();
	}

	// Returns a hash unique hash
	this->hash = VAOManager::get_instance().compute_hash(vertices, indices);

	// If this hash already exists gather IDs and return
	if(VAOManager::get_instance().vao_exists(this->hash)) {
		const VAOManager::VAOEntry entry = VAOManager::get_instance().get_vao_entry(this->hash);
		VAOManager::get_instance().increase_ref_count(this->hash);

		#ifdef SCARAB_DEBUG_VAO_MANAGER
		LOG_DEBUG("Hash %zu found!", hash);
		#endif

		this->vao = new VAO(entry.vao_id);
		this->vbo = new VBO(entry.vbo_id);
		this->ebo = new EBO(entry.ebo_id);

		// No need to make attributes because they are already made
		// Dont know why i am explaining this, its obvious
		this->cleanup_attributes();
		return;
	}

	// Generate VAO using VAO struct to use VAO functions
	this->vao = new VAO(); // Deleted at the end of this function
	this->vao->bind();

	// Gen and bind EBO
	if(!indices.empty()) {
		this->ebo = new EBO(indices);
		this->ebo->bind();
	}

	this->vbo = new VBO();
	this->vbo->bind();
	this->vbo->alloc_data(vertices.size() * sizeof(T), vertices.data());

	// Position
	this->vbo->link_attrib(0, 3, sizeof(T), 0);

	// If T is a Vertex, add the other attributes
	if constexpr (std::is_base_of_v<Vertex, T>) {
		for(size_t i = 0; i < this->attributes.size(); i++) {
			const VertexAttribute attrib = this->attributes[i];
			// +1 to jump position which is always added and already added
			this->vbo->link_attrib(i + 1, attrib.component_count,
					sizeof(T), attrib.offset, attrib.type, attrib.normalized);
		}
	}

	// Unbind all
	this->vao->unbind();
	this->vbo->unbind();
	this->ebo->unbind();

	// LOG_DEBUG("vao id: %u", this->vao->get_id());
	// LOG_DEBUG("vbo id: %u", this->vbo->get_id());
	// LOG_DEBUG("ebo id: %u", this->ebo ? this->ebo->get_id() : 0);

	// Emplace new entry
	VAOManager::get_instance().new_entry(hash, {
		this->vao->get_id(),
		this->vbo->get_id(),
		(this->ebo) ? this->ebo->get_id() : 0
	});

	this->cleanup_attributes();
}
