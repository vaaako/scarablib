#include "scarablib/render/scene.hpp"

bool Scene::remove(const std::string_view key) noexcept {
	auto it = this->lookup.find(key);
	if(it == this->lookup.end()) {
		return false;
	}

	const size_t index = it->second;
	const size_t last  = this->meshes.size() - 1;

	// Swap with last for O(1) erase
	if(index != last) {
		std::swap(this->meshes[index], this->meshes[last]);

		// Fix lookup for swapped element
		for(auto& [key, value] : lookup) {
			if(value == last) {
				value = index;
				break;
			}
		}
	}
	
	this->meshes.pop_back();
	this->lookup.erase(it);

	return true;
}


Mesh* Scene::get(const std::string_view key) noexcept {
	auto it = this->lookup.find(key);
	if(it == this->lookup.end()) {
		return nullptr;
	}
	return this->meshes[it->second].get();
}

const Mesh* Scene::get(const std::string_view key) const noexcept {
	auto it = this->lookup.find(key);
	if(it == this->lookup.end()) {
		return nullptr;
	}
	return this->meshes[it->second].get();
}


// void Scene::reorder() noexcept {
// 	// Reverse lookup
// 	std::unordered_map<const Mesh*, std::string_view> reverse;
// 	reverse.reserve(this->lookup.size());
// 	for(const auto& [key, index] : this->lookup) {
// 		reverse.emplace(this->meshes[index].get(), key);
// 	}
//
// 	// Sort Meshes
// 	std::sort(this->meshes.begin(), this->meshes.end(),
// 	[](const std::unique_ptr<Mesh>& a, const std::unique_ptr<Mesh>& b) {
// 		return a->material->key() < b->material->key();
// 	});
//
// 	// Rebuild lookup map
// 	this->lookup.clear();
// 	this->lookup.reserve(this->meshes.size());
// 	for(size_t i = 0; i < this->meshes.size(); i++) {
// 		this->lookup.emplace(reverse[this->meshes[i].get()], i);
// 	}
// }
