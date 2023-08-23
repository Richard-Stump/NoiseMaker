#include "Perlin.hpp"

#include <cmath>
#include <ctime>


#include "../imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/random.hpp>

Perlin::Perlin()
	: distribution(0.0, 2 * glm::pi<float>())
{

}

float Perlin::sample(float x, float y)
{
	int cellX = glm::floor(x);
	int cellY = glm::floor(y);
	
	float blDot = dotGradient(x, y, cellX,		cellY);
	float brDot = dotGradient(x, y, cellX + 1,	cellY);
	float tlDot = dotGradient(x, y, cellX,		cellY + 1);
	float trDot = dotGradient(x, y, cellX + 1,	cellY + 1);

	float offX = x - cellX;
	float offY = y - cellY;

	float value = glm::mix(
		glm::mix(blDot, brDot, fade(offX)),
		glm::mix(tlDot, trDot, fade(offX)),
		fade(offY)
	);

	return (value + 1) * 0.5f;
}

void Perlin::reseed(int seed)
{
	engine.seed(seed);

	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			float angle = distribution(engine);

			gradients[y][x] = glm::vec2(glm::cos(angle), glm::sin(angle));
		}
	}
}

glm::vec2 Perlin::getGradient(int cellX, int cellY)
{
	int ix = wrap(cellX, 256);
	int iy = wrap(cellY, 256);

	return gradients[iy][ix];
}

float Perlin::dotGradient(float x, float y, int cellX, int cellY)
{
	glm::vec2 grad = getGradient(cellX, cellY);
	glm::vec2 toSample(x - cellX, y - cellY);

	return glm::dot(grad, toSample);
}

