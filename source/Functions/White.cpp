#include "White.hpp"

#include "../imgui/imgui.h"
#include <glm/glm.hpp>


WhiteNoise::WhiteNoise()
	: distribution(0.0f, 1.0f)
{

}

float WhiteNoise::sample(float x, float y)
{
	int sx = wrap(x, 256);
	int sy = wrap(y, 256);

	return preCalc[sy][sx];
}

void WhiteNoise::reseed(int seed)
{
	engine.seed(seed);

	for (int y = 0; y < 256; y++)
		for (int x = 0; x < 256; x++)
			preCalc[y][x] = distribution(engine);
}