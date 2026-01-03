#pragma once

#include "scarablib/typedef.hpp"
#include <functional>

namespace ScarabHash {
	// Makes a hash out of a value and return it.
	// WARNING: Do NOT use raw pointers like `char*` or `const char*`, it will not work.
	// Converting to `std::string` or `std::string_view` is fine though
	template <typename T>
	size_t hash_make(const T& value) noexcept {
		size_t seed = 0;
		std::hash<T> h;
		seed ^= h(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}

	// From boost library.
	// To multiple values, keep passing the same variable as `seed` until you done.
	// Is VERY recommended to initialize `seed` before using it.
	// WARNING: Do NOT use raw pointers like `char*` or `const char*`, it will not work.
	// Converting to `std::string` or `std::string_view` is fine though
	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& value) noexcept {
		// Use golden ration to mix the hash
		std::hash<T> h;
		seed ^= h(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	// Fast, deterministic, non-cryptographic hash for raw bytes.
	// Use this for asset caching (textures, shaders, files).
	// WARNING: Do NOT use for security or long-term persistent IDs
	inline size_t hash_bytes_fnv1a(const void* data, const size_t size) noexcept {
		const uint8* bytes = static_cast<const uint8*>(data);
		uint64 hash = 14695981039346656037ull; // 64-bit offset basis
		for(size_t i = 0; i < size; i++) {
			hash ^= bytes[i];
			hash *= 1099511628211ull; // FNV prime
		}
		return hash;
	}
};
