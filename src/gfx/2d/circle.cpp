#include "scarablib/gfx/2d/circle.hpp"
#include "scarablib/geometry/geometry_factory.hpp"
#include "scarablib/opengl/resourcesmanager.hpp"
#include "scarablib/opengl/shaders.hpp"

Circle::Circle() noexcept
	: Sprite(GeometryFactory::make_rectangle_vertices()) {

	const char* source = R"glsl(
		uniform float blur;

		void mainImage(out vec4 fragcolor, in vec2 texuv) {
			vec4 mixedtex = mix(
				texture(texSampler, texuv),
				texture(texSamplerArray, vec3(texuv, texlayer)),
				mixamount
			);

			// NOTE: Alpha is not really needed, the if-case could be "dist < radius"
			// but i want to keep the blur as an option

			const float radius = 0.5;
			// Distance from the center to the borders
			float dist = length(texuv - 0.5);
			// Smooth interpolation of distance. Keep between radius and raius-blur
			// Clamp blur from 0.001 to 1.0
			float alpha = smoothstep(radius, radius - clamp(blur, 0.001, 1.0), dist);

			if(alpha <= blur) {
				discard;
			}

			fragcolor = vec4(mixedtex.rgb * shapeColor.rgb, shapeColor.a * alpha);
		}
	)glsl";

	this->material->shader = ResourcesManager::get_instance().load_shader_program({
		ResourcesManager::ShaderInfo {
			.source = Shaders::DEFAULT_VERTEX2D,
			.type   = Shader::Type::Vertex
		},
		ResourcesManager::ShaderInfo {
			.source   = source,
			.type     = Shader::Type::Fragment,
			.iscustom = true
		},
	});
}

void Circle::draw_logic() noexcept {
	this->update_model_matrix();

	std::shared_ptr<ShaderProgram> shader = this->material->shader; // cache
	shader->set_float("blur", this->blur);

	// hard coded vertices size
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
