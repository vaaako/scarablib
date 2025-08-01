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
// - CIRCLE_FRAGMENT: Fragment shader for circles
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
			texuv       = vec2(aTex.xy);
		}
	)glsl";


	const char* const DEFAULT_FRAGMENT_BAK = R"glsl(
		#version 330 core

		in vec2 texuv;
		out vec4 FragColor;

		uniform sampler2D texSampler;
		uniform vec4 shapeColor;

		void main() {
			vec4 tex = texture(texSampler, texuv);
			if(tex.a == 0.0) {
				discard;
			}
			FragColor =  tex * (shapeColor.rgba / 255);
		}
	)glsl";

	const char* const DEFAULT_FRAGMENT = R"glsl(
		#version 330 core

		in vec2  texuv;
		out vec4 FragColor;

		uniform vec4 shapeColor;

		uniform sampler2D      texSampler;      // Bound to texture unit 0
		uniform sampler2DArray texSamplerArray; // Bound to texture unit 1
		uniform int   texlayer;                 // If using texSamplerArray
		uniform float mixAmount;                // Blend amount (0.0 = only tex2D, 1.0 = only array)

		void main() {
			// mix(texSampler, texSamplerArray, mixAmount) * shapeColor
			vec4 mix = mix(texture(texSampler, texuv), texture(texSamplerArray, vec3(texuv, texlayer)), mixAmount) * shapeColor;
			if(mix.a == 0.0) {
				discard;
			}
			FragColor = mix;
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

		out vec4 FragColor;
		in vec3 texuv;

		uniform samplerCube samplerSkybox;

		void main() {
			FragColor = texture(samplerSkybox, texuv);
		}
	)glsl";

	const char* const BILLBOARD_VERTEX = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTex;

		out vec2 texuv;

		uniform mat4 view;
		uniform mat4 proj;
		uniform vec3 billboardPos;
		uniform float billboardSize;

		void main() {
			// Extract the right and up vectors from the view matrix
			vec3 right = normalize(vec3(view[0].x, view[1].x, view[2].x));
			vec3 up = normalize(vec3(view[0].y, view[1].y, view[2].y));

			// Reconstruct the billboarded position
			vec3 billboardedPos = billboardPos + (aPos.x * right * billboardSize + aPos.y * up * billboardSize);

			gl_Position = proj * view * vec4(billboardedPos, 1.0);
			texuv       = vec2(aTex.xy);
		}
	)glsl";

	const char* const CIRCLE_FRAGMENT = R"glsl(
		#version 330 core

		out vec4 FragColor;
		in vec2 texuv;

		uniform vec4 shapeColor;

		uniform float blur;

		uniform sampler2D      texSampler;      // Bound to texture unit 0
		uniform sampler2DArray texSamplerArray; // Bound to texture unit 1
		uniform int   texlayer;                 // If using texSamplerArray
		uniform float mixAmount;                // Blend amount (0.0 = only tex2D, 1.0 = only array)

		vec3 circle_area(vec2 texuv, vec2 center, float radius, float blur) {
			// Clamp blur to avoid branching
			blur = clamp(blur, 0.001, 1.0); // value, min, max
			// Distance to the center of the circle
			float distance = length(texuv - center);
			// Use smoothstep for smooth edges, radius and blur are now parameters
			float circle = smoothstep(radius, radius - blur, distance);
			// Return
			return vec3(circle);
		}

		void main() {
			// mix(texSampler, texSamplerArray, mixAmount)
			vec4 mix = mix(texture(texSampler, texuv), texture(texSamplerArray, vec3(texuv, texlayer)), mixAmount);
			// Apply circle as a texture
			// Cut alpha from rectangle to make it circular
			FragColor = vec4(mix.rgb * shapeColor.rgb, mix.a * circle_area(texuv, vec2(0.5, 0.5), 0.5, blur).r);
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
