#pragma once

#include "gtc/epsilon.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/utils/hash.hpp"
#include <functional>

// Mesh data uploaded to the GPU
template <typename T>
struct VertexBase {
	static_assert(
		std::is_same_v<T, vec2<float>> ||
		std::is_same_v<T, vec3<float>>,
		"VertexBase<T>: T must be vec2<float> or vec3<float>"
	);

	// Vertex position
	T position; // 12 bytes
	// Normalized texture coordinates
	vec2<float> texuv = vec2<float>(0.0f);

	bool operator==(const VertexBase& other) const noexcept {
		return glm::all(glm::epsilonEqual(position, other.position, 0.0001f)) &&
			glm::all(glm::epsilonEqual(texuv, other.texuv, 0.0001f));
	}
};

// Aliases
using Vertex   = VertexBase<vec3<float>>;
using Vertex2D = VertexBase<vec2<float>>;

// Be able to use Vertex as key in unordered_map
namespace std {
	template <typename T>
	struct hash<VertexBase<T>> {
		size_t operator()(const VertexBase<T>& vertex) const {
			size_t seed = 0;

			ScarabHash::hash_combine(seed, vertex.position.x);
			ScarabHash::hash_combine(seed, vertex.position.y);
			if constexpr (std::is_same_v<vec3<float>, T>) {
				ScarabHash::hash_combine(seed, vertex.position.z);
			}

			ScarabHash::hash_combine(seed, vertex.texuv.x);
			ScarabHash::hash_combine(seed, vertex.texuv.y);

			return seed;
		}
	};
}


