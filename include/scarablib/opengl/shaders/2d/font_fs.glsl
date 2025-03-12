#version 330 core

in vec2 texCoord;

uniform vec4 shapeColor;
uniform sampler2D texSampler;

// Multiplying by 0.004 gives an approximated result as dividing by 255
vec4 normalized_color(vec4 color) {
	return vec4(color.rgba * 0.004);
}

void main() {
	// Sample red channel (glyph alpha)
	vec4 tex = vec4(texture(texSampler, texCoord).r);
	gl_FragColor = vec4(tex * normalized_color(shapeColor));
}
