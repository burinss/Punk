#version 450 core

out vec4 FragColor;

in vec2 FragUV;

uniform sampler2D billboardTexture;

void main() {
	vec4 color = texture(billboardTexture, FragUV);
	FragColor = color;
}
