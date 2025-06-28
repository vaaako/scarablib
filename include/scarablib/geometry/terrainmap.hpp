#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/gfx/color.hpp"
#include <unordered_map>
#include <vector>


// Takes an image and transforms in a collision map
class TerrainMap {
	public:
		// Map width
		uint32 width;
		// Map height
		uint32 height;
		// 1D map of the map types.
		// Ex: 0 = Empty, 1 = Wall
		std::vector<uint8> terrain_map;
		// Maximum distance between colors to consider similar.
		float color_threshold = 30.0f;
		// A: (255, 0, 0) / B: (250, 5, 0)
		// Threshold = 30
		// Distance ~= 7.07
		// Result: The colors are similar

		// Takes an image and transforms in a collision map
		TerrainMap() noexcept = default;

		// Configures a color tone as a terrain type
		inline void set_terrain_color(const uint8 type, const Color& color) {
			this->terrain_colors[type].emplace_back(color);
		}

		// Configures multiple color tones as a terrain type
		inline void set_terrain_color(const uint8 type, const std::vector<Color>& colors) {
			this->terrain_colors[type] = colors;
		}

		// Returns the terrain type at the given coordinates.
		// 0 always represents an empty space
		inline uint8 get_terrain_at(const vec2<uint32>& position) const noexcept {
			return this->get_terrain_at(position.x, position.y);
		}

		// Returns the terrain type at the given coordinates.
		// 0 always represents an empty space
		inline uint8 get_terrain_at(const uint32 x, const uint32 y) const noexcept {
			return (x >= this->width || y >= this->height) ? 0 : this->terrain_map[y * this->width + x];
		}

		// Loads a file and configure the terrain map.
		// Call this after configuring the map
		void load_file(const char* path);

	private:
		// Query colors
		// Uses a vector of colors because the same terrain type can have multiple colors
		// e.g., (255, 0, 0) or (250, 10, 10) or (200, 0, 0) all mean 'wall' (terrain 1)
		std::unordered_map<uint8, std::vector<Color>> terrain_colors; // Possible colors for type
};
