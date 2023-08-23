#version 330 core

layout(location = 0) in vec2	iPos;
layout(location = 1) in float	iHeight;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

void main(void)
{
	vec4 vertexPos = vec4(iPos.x, iHeight, iPos.y, 1.0);

	gl_Position = matProj * matView * matModel *  vertexPos;
}