#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/gfx/model.hpp"
#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/proper/log.hpp"

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
	this->clear_directional_textures();
}


void Billboard::draw(const Camera& camera, const Shader& shader) noexcept {
	this->update_model_matrix();

	// View matrix
	glm::mat4 view = camera.get_view_matrix();
	// Add perspective
	glm::mat4 proj = camera.get_proj_matrix();

	shader.set_matrix4f("proj", proj);
	shader.set_matrix4f("view", view);

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
	// [ 45, 135] 1 Front
	// [135, 225] 6 Front
	// [225, 315] 7 Front

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


// TODO: Instead of making a whole new texture for flipped ones, send a uniform for shader flip

void Billboard::set_directional_textures(const std::vector<std::pair<const char*, bool>> paths) {
	if(paths.size() < 4 || paths.size() > 8) {
		throw ScarabError("The number of textures must be at least 4 or at most 8");
	}

	// 4 for four directions
	// 5 for flipping textures
	const size_t texture_size = (paths.size() != 4 || paths.size() == 5) ? 8 : 4;

	#ifdef SCARAB_DEBUG_BILLBOARD
	std::vector<const char*> texture_paths;
	texture_paths.resize(texture_size, nullptr);
	#endif

	// Resize so i can check if a index has a texture already
	this->textures.resize(texture_size, nullptr);

	for(size_t i = 0; i < paths.size(); i++) {
		const auto& pair = paths.at(i);

		// LOG_DEBUG("Placing at index %zu", i);

		// Place the texture in the eorrect direction
		this->textures[i] = new Texture({
			.path = pair.first,
			.flip_horizontally = false
		});

		#ifdef SCARAB_DEBUG_BILLBOARD 
		texture_paths[i] = pair.first;
		#endif

		// Check for opposite texture
		if(pair.second) {
			// Calculate the opposite index
			const size_t opposite_index = (texture_size - i);

			if(this->textures.at(opposite_index) != nullptr) {
				throw ScarabError("Bad Configuration: Index %zu already has a texture", opposite_index);
			}

			// Place the texture in the correct direction
			this->textures[opposite_index] = new Texture({
				.path = pair.first,
				.flip_horizontally = true
			});

			#ifdef SCARAB_DEBUG_BILLBOARD 
			texture_paths[opposite_index] = pair.first;
			#endif

			// LOG_DEBUG("Flipping %zu and placing at %zu", i, opposite_index);
		}
	}

	if(this->textures.size() != 4 && this->textures.size() != 8) {
		throw ScarabError("Bad Configuration: The textures final size is %zu, which does not match 4 or 8. \nCheck for flipped textures", this->textures.size());
	}

	#ifdef SCARAB_DEBUG_BILLBOARD 
	LOG_DEBUG("Directions texture paths:");
	for(size_t i = 0; i < texture_paths.size(); i++) {
		LOG_DEBUG("[%i] %s", i, texture_paths.at(i));
	}
	#endif
}

void Billboard::clear_directional_textures() noexcept {
	for(Texture* texture : this->textures) {
		// Just in case
		if(texture != nullptr) {
			delete texture;
		}
	}
	this->textures.clear();
}

