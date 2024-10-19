#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D texSampler;
uniform vec4 shapeColor; // Color defined by user later

uniform float blur;

// Multiplying by 0.004 gives an approximated result as dividing by 255
vec3 normalized_color(vec3 color) {
	return vec3(color.r * 0.004, color.g * 0.004, color.b * 0.004);
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
	// Apply circle as a texture
	// This method only works if the texture coordinates are normalized

	FragColor = vec4(normalized_color(shapeColor.rgb), circle_area(texCoord, vec2(0.5, 0.5), 0.5, blur));
}
