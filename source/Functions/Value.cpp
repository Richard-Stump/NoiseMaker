#include "Value.hpp"

#include <vector>

#include "../imgui/imgui.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/random.hpp>


Value::Value()
: distribution(0.0f, 1.0f)
{

}

float Value::sample(float x, float y) 
{
	int cellX = glm::floor(x);
	int cellY = glm::floor(y);
	
	float blVal = getPrecalcValue(cellX,		cellY);
	float brVal = getPrecalcValue(cellX + 1,	cellY);
	float tlVal = getPrecalcValue(cellX,		cellY + 1);
	float trVal = getPrecalcValue(cellX + 1,	cellY + 1);

	float offX = x - cellX;
	float offY = y - cellY;

	return glm::mix(
		glm::mix(blVal, brVal, fade(offX)),
		glm::mix(tlVal, trVal, fade(offX)),
		fade(offY)
	);
}

void Value::reseed(int seed) 
{
	engine.seed(seed);

	for (int y = 0; y < 256; y++)
		for (int x = 0; x < 256; x++)
			preCalc[y][x] = distribution(engine);
}

float Value::getPrecalcValue(int x, int y)
{
	int ix = wrap(x, 256);
	int iy = wrap(y, 256);

	return preCalc[iy][ix];
}