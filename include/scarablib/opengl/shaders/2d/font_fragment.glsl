#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texSampler;
uniform vec4 shapeColor;

// Multiplying by 0.004 gives an approximated result as dividing by 255
// NOTE: Modified to not return alpha
vec4 normalized_color(vec4 color) {
	return vec4(color.r * 0.004, color.g * 0.004, color.b * 0.004, color.a * 0.004);
}

void main() {
	// Extracts the red channel which stores the glyph's alpha value
	float alpha = texture(texSampler, texCoord).r;
	// vec4(1.0, 1.0, 1.0, alpha): Creates a white color with the sampled alpha
	vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
	// Apply text color
	FragColor = normalized_color(shapeColor) * sampled;
}


