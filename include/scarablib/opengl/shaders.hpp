#pragma once

namespace Shaders {
	const char* const DEFAULT_VERTEX = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTex;

		out vec2 texCoord;

		uniform mat4 mvp;

		void main() {
			gl_Position = mvp * vec4(aPos, 1.0);
			texCoord    = vec2(aTex.xy);
		}
	)glsl";


	const char* const DEFAULT_FRAGMENT = R"glsl(
		#version 330 core

		in vec2 texCoord;
		out vec4 FragColor;

		uniform sampler2D texSampler;
		uniform vec4 shapeColor;

		// Multiplying by 0.004 gives an approximated result as dividing by 255
		vec4 normalized_color(vec4 color) {
			return vec4(color.rgba * 0.004);
		}

		void main() {
			vec4 tex = texture(texSampler, texCoord);
			if(tex.a == 0.0) {
				discard;
			}

			FragColor =  tex * normalized_color(shapeColor.rgba);
		}
	)glsl";

	const char* const SKYBOX_VERTEX = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		out vec3 texCoord;

		uniform mat4 proj;
		uniform mat4 view;

		void main() {
			texCoord = aPos;
			vec4 pos = proj * view * vec4(aPos, 1.0);
			// Draw above
			gl_Position = pos.xyww;
		}
	)glsl";


	const char* const SKYBOX_FRAGMENT = R"glsl(
		#version 330

		out vec4 FragColor;
		in vec3 texCoord;

		uniform samplerCube samplerSkybox;

		void main() {
			FragColor = texture(samplerSkybox, texCoord);
		}
	)glsl";

	const char* const BILLBOARD_VERTEX = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTex;

		out vec2 texCoord;

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
			texCoord    = aTex;
		}
	)glsl";

	const char* const CIRCLE_FRAGMENT = R"glsl(
		#version 330 core

		out vec4 FragColor;
		in vec2 texCoord;

		uniform sampler2D texSampler;
		uniform vec4 shapeColor;

		uniform float blur;

		// Multiplying by 0.004 gives an approximated result as dividing by 255
		vec3 normalized_color(vec3 color) {
			return vec3(color.rgb * 0.004);
		}

		vec3 circle_area(vec2 uv, vec2 center, float radius, float blur) {
			// Clamp blur to avoid branching
			blur = clamp(blur, 0.001, 1.0); // value, min, max

			// Distance to the center of the circle
			float distance = length(uv - center);

			// Use smoothstep for smooth edges, radius and blur are now parameters
			float circle = smoothstep(radius, radius - blur, distance);

			return vec3(circle);
		}

		void main() {
			vec4 tex = texture(texSampler, texCoord);
			if(tex.a == 0.0) {
				discard;
			}

			// Apply circle as a texture
			// Cut alpha from rectangle to make it circular
			FragColor = vec4(tex.rgb * normalized_color(shapeColor.rgb), tex.a * circle_area(texCoord, vec2(0.5, 0.5), 0.5, blur).r);
		}
	)glsl";
};
