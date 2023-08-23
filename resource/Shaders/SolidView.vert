#version 330 core

layout(location = 0) in vec2	iPos;
layout(location = 1) in float	iHeight;

out float	gHeight;
out vec3	gWorldPos;

uniform mat4	matProj;
uniform mat4	matView;
uniform mat4	matModel;

void main(void)
{
	vec4 vertexPos = matModel * vec4(iPos.x, iHeight, iPos.y, 1);

	gHeight		= vertexPos.y;
	gWorldPos	= vertexPos.xyz;

	gl_Position = matProj * matView * vertexPos;
}