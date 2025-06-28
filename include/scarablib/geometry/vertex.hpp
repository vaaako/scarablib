#pragma once

#include "scarablib/typedef.hpp"
#include <functional>
#include <optional>

// Mesh data uploaded to the GPU
struct Vertex {
	// Vertex positions
	vec3<float> position; // 12 bytes
	// Normalized texture coordinates
	vec2<float> texuv;

	// Value: 0-255
	// Optional value, used for Texture Arrays.
	// Set the index of the texture to use for this vertex.
	// Should be the same for all faces.
	// WARNING: Convert this value to float BEFORE PASSING to fragment shader (otherwise it will not work properly)
	// std::optional<uint8> texid; // 2 bytes
	//
	// Value: 0-255
	// Optional value, is used for simple shading
	// std::optional<uint8> shading; // 2 bytes

	bool operator==(const Vertex& other) const noexcept {
		return this->position == other.position &&
			   this->texuv    == other.texuv;
			   // this->texid    == other.texid &&
			   // this->shading  == other.shading;
	}
};

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

			hash_combine(vertex.texuv.x);
			hash_combine(vertex.texuv.y);

			// hash_combine(vertex.texid);
			// hash_combine(vertex.shading);

			return seed;
		}
	};
}


