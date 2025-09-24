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
			texuv       = aTex;
		}
	)glsl";


	const char* const DEFAULT_FRAGMENT_BAK = R"glsl(
		#version 330 core

		in vec2 texuv;
		out vec4 fragcolor;

		uniform sampler2D texSampler;
		uniform vec4 shapeColor;

		void main() {
			vec4 tex = texture(texSampler, texuv);
			if(tex.a == 0.0) {
				discard;
			}
			fragcolor =  tex * (shapeColor.rgba / 255);
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

		void main() {
			// mix(texSampler, texSamplerArray, mixamount)
			vec4 mixtex = mix(texture(texSampler, texuv),
							texture(texSamplerArray, vec3(texuv, texlayer)),
							mixamount);
			if(mixtex.a < 0.001) {
				discard;
			}
			fragcolor = mixtex * shapeColor;
		}
	)glsl";

	// void user_main(in vec2 uv, out vec4 color); <- declared and injected before main()
	// user_main(texuv, fragcolor);                <- called inside main()
	// in: like const
	// out: write-only

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

	const char* const CIRCLE_FRAGMENT = R"glsl(
		#version 330 core

		in vec2 texuv;
		out vec4 fragcolor;

		uniform vec4 shapeColor;

		uniform sampler2D      texSampler;      // Bound to texture unit 0
		uniform sampler2DArray texSamplerArray; // Bound to texture unit 1
		uniform int   texlayer;                 // If using texSamplerArray
		uniform float mixamount;                // Blend amount (0.0 = only tex2D, 1.0 = only array)

		float mask(float radius);

		void main() {
			// mix(texSampler, texSamplerArray, mixamount)

			vec4 mixtex = mix(texture(texSampler, texuv),
							texture(texSamplerArray, vec3(texuv, texlayer)),
							mixamount);

			// NOTE: Alpha is not really needed, the if-case could be "dist < radius"
			// but i want to keep the blur as an option

			const float radius = 0.5;
			// Distance from the center to the borders
			float dist = length(texuv - 0.5);
			// Smooth interpolation of distance. Keep between radius and raius-blur
			// Clamp blur from 0.001 to 1.0
			// float alpha = smoothstep(radius, radius - clamp(blur, 0.001, 1.0), dist);
			float alpha = mask(radius);

			if(alpha <= blur) {
				discard;
			}

			fragcolor = vec4(mixtex.rgb * shapeColor.rgb, shapeColor.a * alpha);
		}
		
		uniform float blur;
		float mask(float radius) {
			// Distance from the center to the borders
			float dist = length(texuv - 0.5);
			// Smooth interpolation of distance. Keep between radius and raius-blur
			// Clamp blur from 0.001 to 1.0
			float alpha = smoothstep(radius, radius - clamp(blur, 0.001, 1.0), dist);
			return alpha;
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
