#pragma once

#include "ext/matrix_float4x4.hpp"

// Shaders in this namespace:
// - DEFAULT_VERTEX: Default vertex shader for meshes
// - DEFAULT_FRAGMENT: Default fragment shader for meshes
//
// - SKYBOX_VERTEX: Vertex shader for skybox
// - SKYBOX_FRAGMENT: Fragment shader for skybox
//
// - BILLBOARD_VERTEX: Vertex shader for billboards
//
// - FONT_FRAGMENT: Fragment shader for fonts
//
// Uniforms in this namespace:
// - Camera: view and proj matrices
// - Mesh: Model matrix and color vector
namespace Shaders {
	struct alignas(16) CameraUniformBuffer {
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct alignas(16) MeshUniformBuffer {
		glm::mat4 model;
	};
	
	struct alignas(16) MaterialUniformBufferr {
		glm::vec4 color;
		glm::vec4 params; // x = mixamount, y = texlayer
	};

	const char* const DEFAULT_VERTEX = R"glsl(
		#version 420 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTex;

		out vec2 texuv;

		layout(std140, binding = 0) uniform Camera {
			mat4 view;
			mat4 proj;
		};

		layout(std140, binding = 1) uniform Model {
			mat4 model;
		};

		void main() {
			gl_Position = proj * view * model * vec4(aPos, 1.0);
			texuv       = aTex;
		}
	)glsl";

	const char* const DEFAULT_VERTEX2D = R"glsl(
		#version 330 core

		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec2 aTex;

		out vec2 texuv;

		uniform mat4 mvp;

		void main() {
			gl_Position = mvp * vec4(aPos, 0.0, 1.0);
			texuv       = aTex;
		}
	)glsl";

	const char* const DEFAULT_FRAGMENT = R"glsl(
		#version 330 core

		in vec2  texuv;
		out vec4 fragcolor;

		uniform vec4 shapeColor;

		uniform sampler2D      texSampler;      // Bound to texture unit 0
		uniform sampler2DArray texSamplerArray; // Bound to texture unit 1
		uniform int   texlayer;                 // If using texSamplerArray
		uniform float mixamount;                // Blend amount (0.0 = only tex2D, 1.0 = only array)

		// User shader will be injected here
		// it will also be injected a "HAS_USER_SHADER" macro
		// {{USER_CODE}}

		#ifndef HAS_USER_SHADER
		void mainImage(out vec4 fragcolor, in vec2 texuv) {
			vec4 mixedtex = mix(
				texture(texSampler, texuv),
				texture(texSamplerArray, vec3(texuv, texlayer)),
				mixamount
			);
			if(mixedtex.a < 0.001) {
				discard;
			}
			fragcolor = mixedtex * shapeColor;
		}
		#endif
		
		void main() {
			mainImage(fragcolor, texuv);
		}
	)glsl";

	const char* const SKYBOX_VERTEX = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		out vec3 texuv;

		uniform mat4 proj;
		uniform mat4 view;

		void main() {
			texuv = aPos;
			vec4 pos = proj * view * vec4(aPos, 1.0);
			// Fixed position
			gl_Position = pos.xyww;
		}
	)glsl";


	const char* const SKYBOX_FRAGMENT = R"glsl(
		#version 330

		out vec4 fragcolor;
		in vec3 texuv;

		uniform samplerCube samplerSkybox;

		void main() {
			fragcolor = texture(samplerSkybox, texuv);
		}
	)glsl";

	const char* const BILLBOARD_VERTEX = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTex;

		out vec2 texuv;

		uniform mat4 view;
		uniform mat4 proj;
		uniform vec3 billpos;   // Billboard Position
		uniform float billsize; // Billboard Size

		void main() {
			// Extract the right and up vectors from the view matrix
			vec3 right = normalize(vec3(view[0].x, view[1].x, view[2].x));
			vec3 up    = normalize(vec3(view[0].y, view[1].y, view[2].y));
			// Reconstruct the billboarded position
			vec3 rbillpos = billpos + (aPos.x * right * billsize + aPos.y * up * billsize);

			gl_Position = proj * view * vec4(rbillpos, 1.0);
			texuv       = vec2(aTex.xy);
		}
	)glsl";

	const char* const FONT_FRAGMENT = R"glsl(
		#version 330 core

		in vec2 texuv;

		uniform vec4 shapeColor;
		uniform sampler2D texSampler;

		void main() {
			// Sample red channel (glyph alpha)
			vec4 tex = vec4(texture(texSampler, texuv).r);
			gl_FragColor = vec4(tex * shapeColor);
		}
	)glsl";
};
