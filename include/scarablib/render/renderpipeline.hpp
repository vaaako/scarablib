#pragma once

#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/render/scene.hpp"

class RenderPipeline {
	public:
		void render(const Scene& scene) noexcept;

	private:
		struct RenderCommand {
			uint64 sort_key;
			Mesh* mesh;
			Material* material;
			glm::mat4 transform;
		};

		// Tracks GL state to avoid redundant API calls
		struct StateCache {
			GLuint cur_program = 0;
			GLuint cur_vao = 0;
			GLuint cur_texture_0 = 0;
			GLuint cur_texture_1 = 0;

			void reset() {
				this->cur_program = 0;
				this->cur_vao = 0;
				this->cur_texture_0 = 0;
				this->cur_texture_1 = 0;
			}
		};

		std::vector<RenderCommand> render_queue;
		StateCache state_cache;


		uint64 frame_counter = 0; // Monotonically increasing
		uint32 frame_index   = 0; // frame_counter % FRAMES_IN_FLIGHT
		uint32 draw_index    = 0;

		inline void begin_frame() noexcept {
			this->render_queue.clear();
			this->state_cache.reset();

			this->frame_index = this->frame_counter % 3;
			this->draw_index  = 0;

			// TODO: Clean screen buffers
		}
		// Submit render command
		void submit(const Mesh& mesh, const Material& material, const glm::mat4 transform);
		// Sorting and drawing phase
		void flush(const Camera& camera);

		void drawmeshes(const std::vector<std::unique_ptr<Mesh>>& meshes) noexcept;
		void bind_material(Material& material) noexcept;


};
