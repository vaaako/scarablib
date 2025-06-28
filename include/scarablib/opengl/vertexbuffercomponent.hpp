#pragma once

#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"

// TODO: In future texuv will not be always presented
// For invisible objects

// A class that bundles the VAO, VBO and EBO buffers.
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

		inline size_t get_hash() const noexcept {
			return this->hash;
		}

		// Configurates additional attributes for data.
		// This will only be used if data is a vector of Vertex.
		// - `count`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `type`: (Default: GL_FLOAT) Type of data (e.g., GL_FLOAT).
		// - `normalized`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
		template<typename T>
		void add_attribute(const uint32 count, const GLenum type = GL_FLOAT, const bool normalized = true);

		// Automatically computes Vertex Buffer and stores in VAOManager
		template <typename T, typename U>
		void make_vao(const std::vector<T>& vertices, const std::vector<U>& indices);

	private:
		struct VertexAttribute {
			const uint32 location;
			const uint32 component_count;
			const GLenum type;
			const bool normalized;
			const size_t offset;
		};
		std::vector<VertexAttribute> attributes;

		// Current offset of attributes
		size_t stride = sizeof(Vertex); // position and texuv are always presented
		size_t hash;

		inline void cleanup_attributes() noexcept {
			// Clear attributes
			this->attributes.clear();
			this->attributes.shrink_to_fit(); // Actually releases the allocated memory
			this->stride = sizeof(Vertex);
		}
};


template<typename T>
void VertexBufferComponent::add_attribute(const uint32 count, const GLenum type, const bool normalized) {

	// Plus 2 because position and texuv are always at location 0 and 1
	const uint32 location = 2 + (uint32)this->attributes.size();
	this->attributes.push_back({ location, count, type, normalized, this->stride });
	this->stride += count * sizeof(float);
}

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

	// If T is a Vertex (3D shape), add the other attributes
	if constexpr (std::is_base_of_v<Vertex, T>) {
		this->vbo->link_attrib(1, 2, sizeof(T), offsetof(T, texuv));

		for(const auto& attrib : this->attributes) {
			this->vbo->link_attrib(attrib.location, attrib.component_count,
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
