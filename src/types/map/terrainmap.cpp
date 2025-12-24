#include "scarablib/types/map/terrainmap.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/gfx/image.hpp"

void TerrainMap::load_file(const char* path) {
	// Flip image so Y-coodinate is correctly: (0, 0) = top-left instead of bottom-left
	// (this just means stb will load image as default, which is flipped vertically)
	Image* image = new Image(path, true, false);
	if(image->data == nullptr) {
		delete image;
		throw ScarabError("Image (%s) was not found", image->path);
	}

	this->width  = static_cast<uint32>(image->width);
	this->height = static_cast<uint32>(image->height);

	// 0 = Empty
	this->terrain_map.resize(this->width * this->height, 0);

	for(uint32 y = 0; y < this->height; y++) {
		for(uint32 x = 0; x < this->width; x++) {
			// Finds position of pixel in data
			const uint32 index = (y * this->width + x) * 3; // 3 because ignoring alpha
			// Color of current pixel
			Color pixel = Color(image->data[index], image->data[index + 1], image->data[index + 2], 255);

			// Best match for this pixel
			uint8 best_match = 0; // 0 is no terrain
			// How close is the pixel to the best match
			float best_distance = this->color_threshold;

			// Loop through all terrain types (Ex: wall, water, grass...)
			for(const auto& [type, ref_color] : this->terrain_colors) {
				// Loop through all color tones of this type
				for(const Color& color : ref_color) {
					// How close is the pixel to the color
					const float distance = pixel.distance_to(color);
					if(distance < best_distance) {
						// Closer than previous best
						best_distance = distance;
						best_match = type;

						// Found perfect match
						if(distance == 0.0f) {
							break;
						}
					}
				}
			}

			// After checking all terrains, if the pixel is close enough to any terrain, set it
			if(best_distance <= this->color_threshold) {
				this->terrain_map[y * this->width + x] = best_match;
			}
		}
	}

	delete image;
}
