#pragma once

// This is a class to make a billboard model.
// WARNING: Do not use this class directly, use ModelFactory::create_billboard(const Model::Config& conf)
#include "scarablib/gfx/model.hpp"
#include "scarablib/utils/file.hpp"

struct Billboard : public Model {
	//          FRONT
	//            ↑
	// FRONT_LEFT   FRONT_RIGHT
	//      ↖           ↗
	// LEFT   ← billboard →   RIGHT
	//      ↙           ↘
	// BACK_LEFT    BACK_RIGHT
	//            ↓
	//           BACK

	// WARNING: Do not use this class directly, use ModelFactory::create_cube(const Model::Config& conf)
	Billboard(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
	// This is overrided because if not, it wouldn't be possible to clean the textures
	~Billboard() noexcept override;

	// This will add multiple textures that changes depending on the angle of the camera relative to the billboard.
	// The first element is the path of the texture.
	// and the second element is the textures to be flipped (0 = flip none).
	// If the texture is flipped, it will be used in the opposite direction.
	// Example: 123 will flip the respective textures to 567.
	// The vector MUST have minimum of 4 elements and maximum of 8 elements
	// WARNING: Please use this method just at the creation of the billboard, if you want to change a texture
	void set_directional_textures(const std::vector<const char*> paths, uint32 flip);



	// Changes the texture based on the angle of the billboard front face relative to the focus position.
	// Just use this method if you have at least 4 textures set.
	// *WARNING:* This method DOES NOT check if textures was set correctly, you will have a error if don't
	inline void rotate_four_directions(const vec3<float>& focus_position) noexcept {
		// [-45,  45] 0 Front
		// [ 45, 135] 1 Right
		// [135, 225] 2 Left
		// [225, 315] 3 Back

		const float angle = this->direction_angle(focus_position);
		const uint32 sector = static_cast<uint32>((angle + 45.0f) / 90.0f) % 4;
		this->set_texture(this->textures[sector]);
	}

	// Changes the texture based on the angle of the billboard front face relative to the focus position.
	// Just use this method if you have 8 textures set.
	// *WARNING:* This method DOES NOT check if textures was set correctly, you will have a error if don't
	inline void rotate_eight_directions(const vec3<float>& focus_position) noexcept {
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

		// NOTE: Eight is conveniently a power of two, so you could use bit manipulation

		const float angle = this->direction_angle(focus_position);
		const uint32 sector = static_cast<uint32>((angle + 22.5f) / 45.0f) % 8;
		if(sector != this->cur_sector) {
			this->set_texture(this->textures[sector]);
			this->cur_sector = sector;
		}

		#ifdef SCARAB_DEBUG_BILLBOARD
		LOG_INFO("Angle: %f, Direction: [%i] %s", angle, sector, str_dirs[sector].c_str());
		#endif
	}

	// Returns the shader used by billboard
	inline Shader* get_shader() const noexcept override {
		return this->shader;
	};

	// Returns the angle of the focus position relative to the billboard.
	// In simple terms: Where the camera is relative to the billboard? 0.0 is in the FRONT and 180.0 is in the BACK
	inline float direction_angle(const vec3<float>& focus_position) const noexcept  {
		// Avoid full 3D vector normalization - only need 2D directions
		return glm::degrees(
			// Only calculate in 2D (xz-plane) since thats all is needed for billboards
			std::atan2(
				focus_position.x - this->position.x,
				focus_position.z - this->position.z
			)
		);

		// vec3<float> direction = focus_position - this->position;
		// direction = glm::normalize(direction);
		// return glm::degrees(std::atan2(direction.x, direction.z));
	}

	private:
		// Only used for directional textures
		std::vector<Texture*> textures;
		uint32 cur_sector; // Save to check and only change texture if needed

		Shader* shader = new Shader(
			FileHelper::read_file(THIS_FILE_DIR + "/../../opengl/shaders/3d/billboard_vertex.glsl").c_str(),
			FileHelper::read_file(THIS_FILE_DIR + "/../../opengl/shaders/3d/fragment.glsl").c_str()
		);

		void draw(const Camera& camera, const Shader& shader) noexcept override;
};
