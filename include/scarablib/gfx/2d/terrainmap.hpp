#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include <unordered_map>
#include <vector>


class TerrainMap {
	public:
		// Takes an image and transforms in a collision map
		TerrainMap() noexcept = default;

		// Returns the width of the map
		inline uint32 get_width() const noexcept {
			return this->width;
		}

		// Returns the height of the map
		inline uint32 get_height() const noexcept {
			return this->height;
		}

		// Returns a 1D array of the map types
		inline std::vector<uint8> get_map() const noexcept {
			return this->terrain_map;
		}

		// Configures a color tone as a terrain type
		inline void set_terrain_color(const uint8 type, const Color& color) {
			this->terrain_colors[type].emplace_back(color);
		}

		// Configures multiple color tones as a terrain type
		inline void set_terrain_color(const uint8 type, const std::vector<Color>& colors) {
			this->terrain_colors[type] = colors;
		}

		// Sets a new maximum distance between colors
		inline void set_color_threshold(const float threshold) {
			this->color_threshold = threshold;
		}

		// Loads a file and configure the terrain map.
		// Call this after configuring the map
		void load_file(const char* path);

		// Returns the terrain type at the given coordinates.
		// 0 always represents an empty space
		inline uint8 get_terrain_at(const uint32 x, const uint32 y) const noexcept {
			if(x >= this->width || y >= this->height) {
				return 0;
			}
			return this->terrain_map[y * this->width + x];
		}

		// Returns the terrain type at the given coordinates.
		// 0 always represents an empty space
		inline uint8 get_terrain_at(const vec2<uint32>& position) const noexcept {
			return this->get_terrain_at(position.x, position.y);
		}

	private:
		uint32 width;
		uint32 height;

		std::vector<uint8> terrain_map; // Ex: 0 = Empty, 1 = Wall...
		// Query colors
		// Uses a vector of colors because the same terrain type can have multiple colors
		// e.g., (255, 0, 0) or (250, 10, 10) or (200, 0, 0) all mean 'wall' (terrain 1)
		std::unordered_map<uint8, std::vector<Color>> terrain_colors; // Possible colors for type

		// Maximum distance between colors to consider similar
		float color_threshold = 30.0f;
		// A: (255, 0, 0) / B: (250, 5, 0)
		// Threshold = 30
		// Distance ~= 7.07
		// Result: The colors are similar
};
