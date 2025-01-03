#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texSampler;
// uniform sampler2D mask; // Texture to go above

uniform vec4 shapeColor; // Color defined by user later


// Multiplying by 0.004 gives an approximated result as dividing by 255
vec4 normalized_color(vec4 color) {
	return vec4(color.r * 0.004, color.g * 0.004, color.b * 0.004, color.a * 0.004);
}

void main() {
	// vec3 baseColor = texture(texSampler, texCoord).rgb;
	// float alpha = texture(mask, texCoord).r;
	// FragColor = vec4(baseColor, alpha);
	// texture->bind(0) // default, texture
	// texture->bind(1) // mask, draw above

	FragColor = texture(texSampler, texCoord) * normalized_color(shapeColor.rgba);
}
