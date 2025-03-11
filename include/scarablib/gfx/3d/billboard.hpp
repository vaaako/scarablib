#pragma once

// This is a class to make a billboard model.
// WARNING: Do not use this class directly, use ModelFactory::create_billboard(const Model::Config& conf)
#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/utils/file.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

// PI * 2
#define M_PI2 6.283185307179586f

// Class for billboard model
struct Billboard : public Model {
	enum Direction : uint8 {
		EAST,
		NORTHEAST,
		NORTH,
		NORTHWEST,
		WEST,
		SOUTHWEST,
		SOUTH,
		SOUTHEAST
	};

	enum : uint8 {
		FOUR_SECTORS = 4,
		EIGHT_SECTORS = 8
	};

	// WARNING: Do not use this constructor, use ModelFactory
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
	void config_directional_textures(const std::vector<const char*> paths, uint32 flip);

	// The directional texture is changed relative to the direction the billboard is facing.
	// num_sectors is the number of textures set in the billboard, please choose either 4 or 8.
	// *WARNING:* This method DOES NOT check if textures was set correctly, you will have a error if don't
	void relative_angle(const vec3<float>& camera_pos, const uint8 num_sectors = 8) noexcept;
	// First arg isnt camera object to give some freedom

	// Returns the current direction the billboard is facing
	inline Billboard::Direction get_direction() const noexcept {
		return this->cur_dir;
	}

	// Set a new direction to the billboard face.
	// This is used in relative_angle
	inline void set_direction(const Billboard::Direction dir) {
		this->cur_dir = dir;
	}

	// Set a texture from the directional textures
	inline void set_dir_texture(const uint32 index) noexcept {
		if(index > this->textures.size()) {
			LOG_ERROR_FN("Index out of range");
			return;
		}
		this->set_texture(this->textures[index]);
	}

	// Returns the shader used by billboard
	inline Shader* get_shader() const noexcept override {
		return ShaderManager::get_instance().get_or_load_shader(
			"billboard",
			FileHelper::read_file("resources/shaders/3d/billboard_vs.glsl").c_str(),
			FileHelper::read_file("resources/shaders/fragment.glsl").c_str()
		);
	};

	private:
		// Precalculated directions
		// M_PI is a predefined macro
		const float directions[8] = {
			0.0f,                // [  0°] EAST        (RIGHT)
			M_PI_4f,             // [ 45°] NORTHEAST   (UP-RIGHT)
			M_PI_2f,             // [ 90°] NORTH       (UP)
			2.356194490192345f,  // [135°] NORTHWEST   (UP-LEFT)     3.0f * M_PI_4f
			M_PIf,               // [180°] WEST        (LEFT)
			3.9269908169872414f, // [225°] SOUTHWEST   (DOWN-LEFT)   5.0f * M_PI_4f
			4.71238898038469f,   // [270°] SOUTH       (DOWN)        3.0f * M_PI_2f
			5.497787143782138f   // [315°] SOUTHEAST   (DOWN-RIGHT)  7.0f * M_PI_4f
		};


		// Directional textures
		std::vector<Texture*> textures;
		Billboard::Direction cur_dir = Billboard::Direction::EAST; // Only used inside relative_angle
		// Save to check and only change texture if needed
		uint32 cur_sector = 9; // This value so that it will always change in first change

		void draw(const Camera& camera, const Shader& shader) noexcept override;
};
