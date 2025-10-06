#pragma once

#include "scarablib/camera/camera.hpp"
#include "scarablib/geometry/mesh.hpp"
#include "scarablib/window/window.hpp"
#include <algorithm> // lower_bound
#include <memory>

// Virtual class used to make Scene2D and Scene3D
class IScene {
	public:
		// Different modes of drawing shapes
		// - `OUTLINEMODE`: draws the outline of the shape
		// - `FILLMODE`: draws the shape as normal
		enum DrawMode : uint32 {
			OUTLINEMODE = GL_LINE,
			FILLMODE = GL_FILL
		};

		// Build scene object using the window object for viewport.
		IScene(Camera& camera) noexcept;
		virtual ~IScene() noexcept;

		// Add a shape to the scene
		template <typename T, typename... Args>
		T* add(const std::string_view& key, Args&&... args);

		// Returns an object by key and dynamically convert it.
		// Returns nullptr if the key does not exist
		template <typename T>
		std::shared_ptr<T> get_by_key(const std::string_view& key) noexcept;

		// Remove object by key
		void remove_by_key(const std::string_view& key);

		// Draw a single model.
		// If the model was added to the scene, it will be drawn twice
		void draw(Mesh& model) const noexcept;

		// Draw all objects added to the scene.
		// This effectively calls Model::draw() for each object.
		// Beware that this method, for efficienty reasons, pass value to the following uniforms:
		// shapeColor, texSampler, texSamplerArray, texLayer, mixAmount.
		// - shapeColor: Only changed when a model has a different color
		// - texSampler: Bound to texture unit 0
		// - texSamplerArray: Bound to texture unit 1
		// - texLayer: The layer to use of the texture array. Only if Model::MaterialComponent::TextureArray is not null
		// - mixAmount: Blend amount of texture and texture array. Only if Model::MaterialComponent::TextureArray is not null
		//
		// You can personalize all of this inside your model's draw method. Since Model::draw() is called after uniform changes.
		// Just overwrite what you want
		void draw_all() const noexcept;

		// Update viewport using window object
		inline void update_viewport(const Window& window) noexcept {
			this->camera.update_viewport(window);
		}

		// Update viewport using custom width and height
		inline void update_viewport(const uint32 width, const uint32 height) noexcept {
			this->camera.update_viewport(width, height);
		}

		// Sets the polygon drawing mode
		inline void set_drawmode(const DrawMode drawmode) noexcept {
			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		}

		// Returns the number of objects in the scene
		inline uint64 length() noexcept {
			return this->scene.size();
		}

	protected:
		Camera& camera;

		// Objects added to the scene, stored as shared pointers for automatic memory management.
		std::unordered_map<std::string_view, std::shared_ptr<Mesh>> scene;
		// Use VAO as ID to track and batch draw meshes with the same VAO.
		std::unordered_map<uint32, std::vector<std::shared_ptr<Mesh>>> vao_groups;
};


template <typename T>
std::shared_ptr<T> IScene::get_by_key(const std::string_view& key) noexcept {
	if(!this->scene.contains(key)) {
		return nullptr;
	}

	// Try to cast the object to the requested type
	auto casted_ptr = std::dynamic_pointer_cast<T>(this->scene.at(key));
	if (!casted_ptr) {
		throw ScarabError("Failed to cast object with key '%s' to type", key.data());
	}

	return casted_ptr;
}

template <typename T, typename... Args>
T* IScene::add(const std::string_view& key, Args&&... args) {
	static_assert(std::is_base_of_v<Mesh, T>,
			"You can only add Mesh types to the scene");

	if(this->scene.contains(key)) {
		throw ScarabError("Key '%s' already exists", key.data());
	}

	std::shared_ptr<Mesh> mesh_ptr = std::make_shared<T>(std::forward<Args>(args)...); // Create new mesh
	this->scene.emplace(key, mesh_ptr); // Used be get_by_key()

	auto& vao_groups = this->vao_groups[mesh_ptr->vertexarray->get_vaoid()];

	// Sort shaders and texture to minimize bind changes.
	// Find the correct position for insertion
	auto it = std::lower_bound(vao_groups.begin(), vao_groups.end(), mesh_ptr,
		[](const std::shared_ptr<Mesh>& a, const std::shared_ptr<Mesh>& b) {
			const uint32 sa = a->material->shader->get_programid();
			const uint32 sb = b->material->shader->get_programid();

			// Primary sort key: shader pointer
			// This handles null cases too
			if(sa != sb) {
				// Compare even if one is nullptr
				return std::less<const uint32>()(sa, sb);
			}

			// Secondary sort key: texture pointer
			// Only checked if the shaders are the same
			// Texture will never be nullptr in this case becaue is a TextureHandle
			return a->material->texture->get_id() < b->material->texture->get_id();
	});

	// Insert the model in the correct place
	vao_groups.insert(it, mesh_ptr);
	return static_cast<T*>(mesh_ptr.get());
}
