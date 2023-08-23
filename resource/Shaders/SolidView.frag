#version 330 core

in float	fHeight;
in vec3		fNorm;
in vec3		fNorm1;

out vec4 oColor;

const vec3 lightDir = vec3(-1.0, -1.0, -1.0);

const vec3 botColor = vec3(0.1, 0.5, 0.1);
const vec3 midColor = vec3(0.8, 0.6, 0.6);
const vec3 topColor = vec3(0.9, 0.9, 0.9);

float fade(float t)
{
	return 3 * t * t - 2 * t * t * t;
}

vec3 heightColor(float height)
{
	if(height <= 0.0) {
		return botColor;
	}
	if(height <= 1.0) {
		float t = fade(height);
		return mix(botColor, midColor, t);
	}
	else if(height <= 2.0) { 
		float t = fade( height - 1.0);
		return mix(midColor, topColor, t);
	}
	else {
		return topColor;
	}
}

void main(void)
{
	vec3 color = heightColor(fHeight);

	float dp = dot(normalize(fNorm1), normalize(lightDir));
	float strength = clamp(dp, 0.4, 1);

	oColor = vec4(color * strength, 1.0);
}