#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texSampler;
uniform vec4 shapeColor;


// Multiplying by 0.004 gives an approximated result as dividing by 255
// NOTE: Modified to not return alpha
vec4 normalized_color(vec4 color) {
	return vec4(color.r * 0.004, color.g * 0.004, color.b * 0.004, color.a * 0.004);
}

void main() {
	// Extracts the red channel which stores the glyph's alpha value
	// vec4(1.0, 1.0, 1.0, alpha): Creates a white color with the sampled alpha
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texSampler, texCoord).r);
	FragColor = vec4(normalized_color(shapeColor)) * sampled;
}


