#pragma once

#include "scarablib/render/renderpipeline.hpp"

class Renderer {
	public:
		// Add a shape to the scene
		template <typename T, typename... Args>
		inline T& add(const std::string_view& key, Args&&... args) const noexcept {
			return this->scene->add<T>(key, std::forward<Args>(args)...);
		}

		inline void set_camera(Camera* camera) {
			this->scene->active_camera = camera;
		}

		// inline void set_scene(Scene* scene) noexcept {
		// 	delete this->scene;
		// 	this->scene = scene;
		// }

		inline void draw() noexcept {
			this->pipeline.render(*this->scene);
		}

	private:
		Scene* scene = new Scene();
		RenderPipeline pipeline;
};
