#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/gfx/model.hpp"
#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/proper/log.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <optional>

#ifdef SCARAB_DEBUG_BILLBOARD
std::string str_dirs[8] = {
	"FRONT",
	"FRONT_RIGHT",
	"RIGHT",
	"BACK_RIGHT",
	"BACK",
	"BACK_LEFT",
	"LEFT",
	"FRONT_LEFT"
};
#endif


Billboard::Billboard(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(conf, vertices, indices) {}

// Overrided from Mesh, so is needed to release the vao_mesh
// This is overrided because if not, it wouldn't be possible to clean the textures
Billboard::~Billboard() noexcept {
	// Release current vao
	// Mesh does this, but this is a overrided destructor
	VAOManager::get_instance().release_vao(vao_hash);
	delete this->shader;

	// Clear directional textures if any
	if(!this->textures.empty()) {
		for(const Texture* tex : this->textures) {
			if(tex != nullptr) {
				delete tex;
			}
		}
		this->textures.clear();
	}
}


void Billboard::draw(const Camera& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	shader.set_matrix4f("proj", camera.get_proj_matrix());
	shader.set_matrix4f("view", camera.get_view_matrix());

	// Billboard stuff
	shader.set_vector3f("billboardPos", this->conf.position);
	shader.set_float("billboardSize", this->conf.scale.x);

	shader.set_color("shapeColor", this->conf.color);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);
	this->texture->unbind();
}

// Rotate 4 directions
void Billboard::rotate_four_directions(const vec3<float>& focus_position) noexcept {
	// [-45,  45] 0 Front
	// [ 45, 135] 1 Right
	// [135, 225] 2 Left
	// [225, 315] 3 Back

	const float angle = this->direction_angle(focus_position);
	const uint32 sector = static_cast<uint32>((angle + 45.0f) / 90.0f) % 4;
	this->set_texture(this->textures.at(sector));
}

// TODO: Instead of set_texture a more optimized method
//	+ Maybe changing texture_id
//	+ Use texture_id on Model instead of Texture object

// Rotate 8 directions
void Billboard::rotate_eight_directions(const vec3<float>& focus_position) noexcept {
	const float angle = this->direction_angle(focus_position);

	// 8 Directions: Divide the circle into 8 sectors of 45° each
	// Sector mapping:
	// [-180°, -135°) 0 Front
	// [-135°,  -90°) 1 Front-Right
	// [ -90°,  -45°) 2 Right
	// [ -45°,    0°) 3 Back-Right
	// [   0°,   45°) 4 Back
	// [  45°,   90°) 5 Back-Left
	// [  90°,  135°) 6 Left
	// [ 135°,  180°) 7 Front-Left

	// Angle range is [-180°, 180°]
	//   0° is Front (facing toward the camera)
	// 180° is Back  (facing away from the camera)

	// Sector Calculation Logic:
	// 1. The input angle is in the range [-180°, 180°]
	// 2. Each sector spans by 45 degrees (360 / 8 sectors = 45)
	// 3. To align the sectors, add 22.5° to the angle (half the sector size)
	//    - This centers the sectors around the angle range.
	// 3. Divide by 45° (sector size) to determine the sector index.
	// 4. Use modulo 8 to wrap around the index if it exceeds 7

	// What is the Span?
	// - The span is the angular size of each sector.
	// - For 8 directions, the span is 45° (360° / 8 sectors).
	// - The span determines how wide each sector is and is used to map the angle to the correct sector.

	// Why 22.5°?
	// - Each sector spans 45°, so half of that (22.5°) is added to align the sectors.
	// - This ensures that the sector boundaries are correctly centered.
	// - Formula: SP (Span) = (360 / N) = Where N is the number of sectors
	//    - Sector = (A + (SP / 2) / SP) % N

	// Example:
	// -180° + 22.5° = -157.5° → Sector 0 (Front)
	//    0° + 22.5° =   22.5° → Sector 4 (Back)
	//  180° + 22.5° =  202.5° → Wraps to 22.5° → Sector 0 (Front)
	const uint32 sector = static_cast<uint32>((angle + 22.5f) / 45.0f) % 8;
	this->set_texture(this->textures.at(sector));

	#ifdef SCARAB_DEBUG_BILLBOARD
	LOG_INFO("Angle: %f, Direction: [%i] %s", angle, sector, str_dirs[sector].c_str());
	#endif
}

#define SCARAB_DEBUG_BILLBOARD_TEXTURE

void Billboard::set_directional_textures(const std::vector<const char*> paths, uint32 flip) {
	// Get length fron int
	const size_t fliplength = (flip == 0) ? 0 : static_cast<size_t>(std::log10(std::abs(static_cast<int>(flip))) + 1);
	const size_t final_size = paths.size() + fliplength; // Track opposite length

	if(paths.size() < 4 && paths.size() > 8) {
		throw ScarabError("The number of images must be at least 4 or at most 8");
	}

	if(fliplength > paths.size()) {
		throw ScarabError("Can't flip %i images, since there are only %zu images", fliplength, paths.size());
	}

	if(final_size != 4 && final_size != 8) {
		throw ScarabError("The total number of textures after flip must be 4 or 8. You passed %zu images and %zu images to flip.", paths.size(), fliplength);
	}

	// Extract digits from flip
	std::vector<size_t> digits;
	while(flip > 0) {
		digits.push_back(flip % 10); // Push last
		flip /= 10; // Remove last
	}
	// Sort crescent
	std::sort(digits.begin(), digits.end());

	// Resize and init values with nullptr so i can check if a index has a texture already
	this->textures.resize(final_size, nullptr);

	for(size_t i = 0; i < paths.size(); i++) {
		// Add current config
		this->textures[i] = new Texture({
			.path = paths.at(i),
			.flip_horizontally = false
		});

		// If index is in digits, flip and add to configs
		if(digits.size() > 0 && digits.front() == i + 1) {
			const size_t opposite_index = final_size - i;
			#ifdef SCARAB_DEBUG_BILLBOARD_TEXTURE
			LOG_DEBUG("Flipping %zu and placing at %zu", i, opposite_index);
			#endif

			// Add config for opposite texture
			this->textures[opposite_index] = new Texture({
				.path = paths.at(i),
				.flip_horizontally = true
			});

			digits.erase(digits.begin());
		}
	}

	// Check if its all correct
	for(size_t i = 0; i < this->textures.size(); i++) {
		if(this->textures[i] == nullptr) {
			throw ScarabError("Bad Configuration: The texture at index %zu is nullptr. The final quantity of textures must be 4 or 8. This might be due to flipped texture bad configuration", i);
		}
	}
}


// void Billboard::set_directional_textures(const std::vector<const char*> paths, uint32 flip) {
// 	// How many to flip
// 	const size_t fliplength = flip == 0 ? 0 : static_cast<size_t>(std::log10(std::abs(static_cast<int>(flip))) + 1);
// 	const size_t final_size = paths.size() + fliplength; // Track opposite length
//
// 	if(paths.size() < 4 && paths.size() > 8) {
// 		throw ScarabError("The number of images must be at least 4 or at most 8");
// 	}
//
// 	if(fliplength > paths.size()) {
// 		throw ScarabError("Can't flip %i images, since there are only %zu images", fliplength, paths.size());
// 	}
//
// 	if(final_size != 4 && final_size != 8) {
// 		throw ScarabError("The total number of textures after flip must be 4 or 8. You passed %zu images and %zu images to flip.", paths.size(), fliplength);
// 	}
//
// 	// Extract digits from flip
// 	std::vector<size_t> digits;
// 	while(flip > 0) {
// 		digits.push_back(flip % 10); // Push last
// 		flip /= 10; // Remove last
// 	}
// 	// Sort crescent
// 	std::sort(digits.begin(), digits.end());
//
// 	// Will be used to make the TextureArray
// 	std::vector<TextureArray::Config> configs;
// 	// Resize and init values with nullptr so i can check if a index has a texture already
// 	configs.resize(final_size, {});
//
// 	for(size_t i = 0; i < paths.size(); i++) {
// 		// Add current config
// 		configs[i] = TextureArray::Config {
// 			.path = paths.at(i),
// 			.flip_horizontally = false
// 		};
//
// 		// If number is in digits, flip and add to configs
// 		if(digits.size() > 0 && digits.front() == i + 1) {
// 			const size_t opposite_index = final_size - i - 1;
// 			#ifdef SCARAB_DEBUG_BILLBOARD_TEXTURE
// 			LOG_DEBUG("Flipping %zu and placing at %zu", i, opposite_index);
// 			#endif
//
// 			// Add config for opposite texture
// 			configs[opposite_index] = TextureArray::Config {
// 				.path = paths.at(i),
// 				.flip_horizontally = true
// 			};
//
// 			digits.erase(digits.begin());
// 		}
// 	}
//
// 	#ifdef SCARAB_DEBUG_BILLBOARD_TEXTURE
// 	for(size_t i = 0; i < configs.size(); i++) {
// 		LOG_DEBUG("[%zu]: %s", i, configs.at(i).path);
// 	}
// 	#endif
//
// 	// Check if is all correct
// 	for(const TextureArray::Config& conf : configs) {
// 		// Will be null because all configs are inited as {}
// 		if(conf.path == NULL) {
// 			throw ScarabError("Bad Configuration: The final quantity of textures must be 4 or 8. This might be due to flipped texture bad configuration");
// 		}
// 	}
//
// 	this->texture_array = new TextureArray(configs);
// }

