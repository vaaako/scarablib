#pragma once


// Shaders in this namespace:
// - DEFAULT_VERTEX: Default vertex shader for meshes
// - DEFAULT_FRAGMENT: Default fragment shader for meshes
//
// - SKYBOX_VERTEX: Vertex shader for skybox
// - SKYBOX_FRAGMENT: Fragment shader for skybox
//
// - BILLBOARD_VERTEX: Vertex shader for billboards
//
// - FONT_FRAGMENT: (unfinished) Fragment shader for fonts
namespace Shaders {
	const char* const DEFAULT_VERTEX = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTex;

		out vec2 texuv;

		uniform mat4 mvp;

		void main() {
			gl_Position = mvp * vec4(aPos, 1.0);
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

		// Multiplying by 0.004 gives an approximated result as dividing by 255
		vec4 normalized_color(vec4 color) {
			return vec4(color.rgba * 0.004);
		}

		void main() {
			// Sample red channel (glyph alpha)
			vec4 tex = vec4(texture(texSampler, texuv).r);
			gl_FragColor = vec4(tex * normalized_color(shapeColor));
		}
	)glsl";
};
