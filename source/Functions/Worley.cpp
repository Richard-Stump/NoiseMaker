#include "Worley.hpp"


#include "../imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/random.hpp>


Worley::Worley()
	: distribution(0.0f, 1.0f)
{

}

float Worley::sample(float x, float y)
{
	int cellX = glm::floor(x);
	int cellY = glm::floor(y);

	float minDistance = 10.0f;

	for (int iy = -1; iy <= 1; iy++) {
		for (int ix = -1; ix <= 1; ix++) {
			glm::vec2 point = getPoint(cellX + ix, cellY + iy);

			float distance = glm::distance(point, { x, y });

			if (distance < minDistance)
				minDistance = distance;
		}
	}

	return minDistance;
}

void Worley::reseed(int seed)
{
	engine.seed(seed);

	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			points[y][x] = {
				distribution(engine),
				distribution(engine)
			};
		}
	}
}


glm::vec2 Worley::getPoint(int cellX, int cellY)
{
	int ix = wrap(cellX, 256);
	int iy = wrap(cellY, 256);

	return glm::vec2(cellX, cellY) + points[iy][ix];
}