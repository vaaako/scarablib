#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texSampler;
uniform vec4 shapeColor; // Color is defined by user later

// Multiplying by 0.004 gives an approximated result as dividing by 255
vec4 normalized_color(vec4 color) {
	return vec4(color.r * 0.004, color.g * 0.004, color.b * 0.004, color.a * 0.004);
}

void main() {
	FragColor = texture(texSampler, texCoord) * normalized_color(shapeColor.rgba);
}
