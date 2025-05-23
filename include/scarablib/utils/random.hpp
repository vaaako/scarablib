#pragma once

#include "scarablib/typedef.hpp"
#include <vector>
#include <random>

namespace ScarabRandom {
	namespace {
		// Helper to access a single static generator
		std::random_device rd;
		std::mt19937 gen(rd());
	}

	// Randomly choose an element from the given vector
	template <typename T>
	[[nodiscard]] T choose_from(const std::vector<T>& choices) noexcept ;

	// - `choices` are the elements to choose from.
	// - `weights` are the corresponding weights.
	// `choices` and `weights` must have the same non-zero size, function will return 0 if this is not the case.
	// Example: `choose_weighted({ 0, 1, 2 }, { 8, 1, 1 })`.
	//
	// 0 has a weight of 8, 1 has a weight of 1, and 2 also has a weight of 1.
	// This means 0 is more likely to be chosen than 1 or 2.
	template <typename T>
	[[nodiscard]] T choose_weighted(const std::vector<T>& choices, const std::vector<uint8>& weights) noexcept ;

	// Choose a random integer in a given range
	[[nodiscard]] inline int inrange(const int min, const int max) noexcept {
		std::uniform_int_distribution<> dis(min, max);
		return dis(gen);
	};

	// Choose a random float in a given range
	[[nodiscard]] inline float inrangef(const float min, const float max) noexcept {
		std::uniform_real_distribution<> dis(min, max);
		return (float)dis(gen);
	};
};

template <typename T>
T ScarabRandom::choose_from(const std::vector<T>& choices) noexcept {
	// Initialize distribution
	std::uniform_int_distribution<> dis(0, (int)(choices.size() - 1));
	// Select a random index and return the corresponding choice
	return choices[dis(gen)];
}

template <typename T>
T ScarabRandom::choose_weighted(const std::vector<T>& choices, const std::vector<uint8>& weights) noexcept {
	// Ensure choices and weights have the same size
	if(choices.size() != weights.size() || choices.empty()) {
		return 0;
	}

	// Initialize distribution
	std::discrete_distribution<> dis(weights.begin(), weights.end());
	// Select a weighted random index and return the corresponding choice
	return choices[dis(gen)];
}
