#pragma once

#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/typedef.hpp"

// The library has a "Builder" and a "Object class" for every object that needs a manager.
// It would totally be possible to put the Builder's methods inside the Object class.
// But the Builder only contains methods and member that are useful when making this object
// then putting this methods all together is kinda pointless and memory loss

// A class that bundles the VAO, VBO and EBO buffers.
// Also stores information necessary for rendering like indices size and indices type.
// With utilitary methods for creating the Vertex Buffer
class VertexArrayBuilder {
	public:
		VertexArrayBuilder() noexcept = default;
		~VertexArrayBuilder() noexcept;

		// Configurates additional attributes for data.
		// This will only be used if data is a vector of Vertex (or child of).
		// - `count`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `normalized`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
		template <typename T>
		void add_attribute(const uint32 count, const bool normalized = false) noexcept;

		// Automatically computes Vertex Buffer and stores in VAOManager
		template <typename T>
		std::shared_ptr<VertexArray> make_vao(const std::vector<Vertex>& vertices, const std::vector<T>& indices = {});

	private:
		std::vector<VAOManager::VertexAttribute> attributes;

		// Current offset of attributes
		size_t stride = sizeof(vec3<float>); // position is always presented

		inline void cleanup_attributes() noexcept {
			// Clear attributes
			this->attributes.clear();
			this->attributes.shrink_to_fit(); // Actually releases the allocated memory
			this->stride = sizeof(vec3<float>);
		}
};

template <typename T>
void VertexArrayBuilder::add_attribute(const uint32 count, const bool normalized) noexcept {
	this->attributes.push_back(VAOManager::VertexAttribute {
		.component_count = count,
		.type            = ScarabOpenGL::gl_type<T>(),
		.normalized      = normalized,
		.offset          = this->stride
	});
	this->stride += count * sizeof(T);
}

template <typename T>
std::shared_ptr<VertexArray> VertexArrayBuilder::make_vao(const std::vector<Vertex>& vertices, const std::vector<T>& indices) {
	// The component's only job is to ask the manager for a VAO
	std::shared_ptr<VertexArray> vertexarray
		= VAOManager::get_instance().acquire_vao(vertices, indices, this->attributes);

	// Cleanup attributes
	this->cleanup_attributes();

	return vertexarray;
}

