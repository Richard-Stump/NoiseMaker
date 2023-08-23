#version 330 core

out vec4 oColor;

const vec3 defColor = vec3(0.75, 0.75, 0.75);

void main(void)
{
	oColor = vec4(defColor, 1);
}