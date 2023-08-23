#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in float	gHeight[];
in vec3		gWorldPos[];

out float	fHeight;
out vec3	fNorm;
out vec3	fNorm1;

void main(void)
{
	vec4 v1 = gl_in[0].gl_Position;
	vec4 v2 = gl_in[1].gl_Position;
	vec4 v3 = gl_in[2].gl_Position;

	vec3 ba = (v2 - v1).xyz;
	vec3 ca = (v3 - v2).xyz;

	fNorm = normalize(cross(ba, ca));
	
	vec3 wv1 = gWorldPos[0];
	vec3 wv2 = gWorldPos[1];
	vec3 wv3 = gWorldPos[2];

	vec3 wba = (wv2 - wv1);
	vec3 wca = (wv3 - wv1);

	fNorm1 = normalize(cross(wba, wca));

	// Emit the vertices exactly as they were.
	gl_Position = v1;
	fHeight = gHeight[0];
	EmitVertex();
	gl_Position = v2;
	fHeight = gHeight[1];
	EmitVertex();
	gl_Position = v3;
	fHeight = gHeight[2];
	EmitVertex();
	EndPrimitive();
}