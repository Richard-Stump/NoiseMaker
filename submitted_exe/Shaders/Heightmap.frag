#version 330 core

in vec2 fTex;

out vec4 oColor;

uniform sampler2D tex;

void main(void) {
	float height = texture(tex, fTex).r;
	oColor = vec4(height, height, height, 1.0);
}