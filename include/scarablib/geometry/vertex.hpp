#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/utils/hash.hpp"
#include <functional>

// Mesh data uploaded to the GPU
struct Vertex {
	// Vertex positions
	vec3<float> position; // 12 bytes
	// Normalized texture coordinates
	vec2<float> texuv = vec2<float>(0.0f);

	bool operator==(const Vertex& other) const noexcept {
		return this->position == other.position &&
			   this->texuv    == other.texuv;
	}
};

// Be able to use Vertex as key in unordered_map
namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(const Vertex& vertex) const {
			size_t seed = 0;

			ScarabHash::hash_combine(seed, vertex.position.x);
			ScarabHash::hash_combine(seed, vertex.position.y);
			ScarabHash::hash_combine(seed, vertex.position.z);

			ScarabHash::hash_combine(seed, vertex.texuv.x);
			ScarabHash::hash_combine(seed, vertex.texuv.y);

			return seed;
		}
	};
}


