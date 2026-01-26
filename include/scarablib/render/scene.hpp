#pragma once

#include "scarablib/camera/camera.hpp"
#include "scarablib/geometry/mesh.hpp"
#include "scarablib/proper/error.hpp"
#include <iterator>
#include <string_view>
#include <utility>

class Scene {
	public:
		// Add a Mesh to the Scene.
		// Throws error if key already exists
		template<typename T, typename... Args>
		T& add(const std::string_view key, Args&&... args);

		// Remove a Mesh from the Scene.
		// Returns `false` if key was found
		bool remove(const std::string_view key) noexcept;

		// Returns object as Mesh class.
		// Returns `nullptr` if not found
		Mesh* get(const std::string_view key) noexcept;
		// Returns object as Mesh class.
		// Returns `nullptr` if not found
		const Mesh* get(const std::string_view key) const noexcept;

		// Returns object as the desired object type
		template<typename T>
		T* get_as(const std::string_view key);

		// Reorder all objects inside the Scene.
		// WARNING:
		// If a material is modified, `Scene::reorder()` MUST be called
		// before rendering, otherwise render order is incorrect
		// void reorder() noexcept;
		// TODO: More optimal would be cached keys and dirty flags


		// Returns true if scene contains the key
		inline bool contains(const std::string_view key) const noexcept {
			return this->lookup.contains(key);
		}

		// Current size of objects in the scene
		inline size_t size() const noexcept {
			return this->meshes.size();
		}
	public:
		Camera* active_camera;
		std::vector<std::unique_ptr<Mesh>> meshes;

	private:
		// Used to look up for a mesh
		std::unordered_map<std::string_view, size_t> lookup;
		// Value is size_t so the deletion and get are more optimized

};

template<typename T, typename... Args>
T& Scene::add(const std::string_view key, Args&&... args) {
	static_assert(std::is_base_of_v<Mesh, T>, "Object must derive from Mesh");

	// Prevent duplicate names
	if(this->lookup.contains(key)) {
		throw ScarabError("Scene already contains mesh with this key");
	}

	std::unique_ptr<Mesh> mesh = std::make_unique<T>(std::forward<Args>(args)...);

	// Find insertion point
	auto it = std::lower_bound(this->meshes.begin(), this->meshes.end(), mesh,
	[](const std::unique_ptr<Mesh>& a, const std::unique_ptr<Mesh>& b) {
		const uint32 sa = a->material->shader->get_programid();
		const uint32 sb = b->material->shader->get_programid();

		// Secondary sort key
		if(sa != sb) {
			// Compare even if one is nullptr
			return std::less<const uint32>()(sa, sb);
		}

		return a->material->texture->get_id() < b->material->texture->get_id();
	});

	// Get index of the inserted mesh
	const size_t index = std::distance(meshes.begin(), it);
	this->meshes.insert(it, std::move(mesh)); // Put on index
	this->lookup.emplace(key, index);

	// Fix indices after insertion
	for(auto& [k, idx] : this->lookup) {
		if(idx >= index && k != key) {
			idx++;
		}
	}
	return static_cast<T&>(*this->meshes[index]);
}

template<typename T>
T* Scene::get_as(const std::string_view key) {
	static_assert(std::is_base_of_v<Mesh, T>, "Type must derive from Mesh");
	return dynamic_cast<T*>(this->get(key));
}

// template<typename T>
// T& Scene::get_as(const std::string_view name) {
// 	static_assert(std::is_base_of_v<Mesh, T>, "Type must derive from Mesh");
//
// 	Mesh* base = this->get(name);
// 	assert(base && "Mesh not found");
//
// 	T* casted = dynamic_cast<T*>(base);
// 	assert(casted && "Mesh type mismatch");
//
// 	return *casted;
// }
