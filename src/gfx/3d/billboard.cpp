#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/gfx/model.hpp"
#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/proper/log.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>

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


Billboard::Billboard(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(vertices, indices) {}

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
	shader.set_vector3f("billboardPos", this->position);
	shader.set_float("billboardSize", this->scale.x);

	shader.set_color("shapeColor", this->color);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);
	this->texture->unbind();
}


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

