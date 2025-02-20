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
	Billboard(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
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
	void rotate_four_directions(const vec3<float>& focus_position) noexcept;

	// Changes the texture based on the angle of the billboard front face relative to the focus position.
	// Just use this method if you have 8 textures set.
	// *WARNING:* This method DOES NOT check if textures was set correctly, you will have a error if don't
	void rotate_eight_directions(const vec3<float>& focus_position) noexcept;

	// Returns the shader used by billboard
	inline Shader* get_shader() const noexcept override {
		return this->shader;
	};

	// Returns the angle of the focus position relative to the billboard.
	// In simple terms: Where the camera is relative to the billboard? 0.0 is in the FRONT and 180.0 is in the BACK
	inline float direction_angle(const vec3<float>& focus_position) const noexcept  {
		vec3<float> direction = focus_position - this->conf.position;

		// Avoid normalizing a zero vector
		if(glm::length(direction) == 0.0f) {
			return 0.0f;
		}

		direction = glm::normalize(direction);
		return glm::degrees(std::atan2(direction.x, direction.z));
	}

	private:
		// Only used for directional textures
		std::vector<Texture*> textures;

		Shader* shader = new Shader(
			FileHelper::read_file(THIS_FILE_DIR + "/../../opengl/shaders/3d/billboard_vertex.glsl").c_str(),
			FileHelper::read_file(THIS_FILE_DIR + "/../../opengl/shaders/3d/fragment.glsl").c_str()
		);

		void draw(const Camera& camera, const Shader& shader) noexcept override;
};
