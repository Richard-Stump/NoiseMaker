#version 330 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTex;

out vec2 fTex;

void main(void) {
	gl_Position = vec4(iPos, 0.0f, 1.0f);
	fTex = iTex;
}