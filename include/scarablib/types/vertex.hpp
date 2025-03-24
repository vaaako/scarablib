#pragma once

#include "scarablib/typedef.hpp"
#include <functional>

// Mesh data uploaded to the GPU
struct Vertex {
	// Vertex positions
	vec3<float> position; // 12 bytes
	// Normalized texture coordinates
	vec2<float> texuv;

	// Value: 0-255 (ignored if not in this range)
	// Optional value, used for Texture Arrays.
	// Set the index of the texture to use for this vertex.
	// Should be the same for all faces.
	// If some vertex of the mesh has this field not in range (0-255), this field will not be uploaded for any face
	int16 texid = -1; // 2 bytes (optional)

	// Value: 0.0-1.0 (ignored if not in this range)
	// Optional value, is used for simple shading.
	// If some vertex of the mesh has this field not in range (0.0f-1.0f), this field will not be uploaded for any face
	float shading = -1.0f; // 4 bytes (optional)

	bool operator==(const Vertex& other) const noexcept {
		return position == other.position &&
			   texuv == other.texuv &&
			   texid == other.texid &&
			   shading == other.shading;
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

			hash_combine(vertex.texid);
			hash_combine(vertex.shading);

			return seed;
		}
	};
}


