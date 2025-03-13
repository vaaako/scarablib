#pragma once

#include "scarablib/typedef.hpp"
#include <vector> // For hash

// Used for setting coordinates for vertices position, texture and normals
struct Vertex {
	vec3<float> position;
	vec3<float> texuv; // Use z = 0 for 2D, z = layer for texture arrays
	// vec3<float> normal;

	bool operator==(const Vertex& other) const noexcept {
		return this->position == other.position
			&& this->texuv == other.texuv;
			// && this->normal == other.normal;
	}
};

// // Used for loading models
// struct Face {
// 	// Init 3 spaces with zero
// 	std::vector<int> vertex_index = std::vector<int>(3);
// 	std::vector<int> texuv_index  = std::vector<int>(3);
// };


// Be able to use Vertex as key in unordered_map
namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(const Vertex& vertex) const {
			size_t seed = 0;

			// Hash position
			auto hash_combine = [&seed](const auto& value) {
				// Use golden ration to mix the hash
				seed ^= hash<decay_t<decltype(value)>>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			};

			hash_combine(vertex.position.x);
			hash_combine(vertex.position.y);
			hash_combine(vertex.position.z);

			// Hash texture coordinates
			hash_combine(vertex.texuv.x);
			hash_combine(vertex.texuv.y);

			// hash_combine(vertex.normal.x);
			// hash_combine(vertex.normal.y);
			// hash_combine(vertex.normal.z);

			return seed;
		}
	};
}

