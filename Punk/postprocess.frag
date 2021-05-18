#version 450 core

out vec4 FragColor;

in vec2 FragUV;

uniform sampler2D colorSampler;

void main() {
	vec2 texCoord = FragUV;
	vec4 color = texture(colorSampler, texCoord);
	FragColor = color;
}
