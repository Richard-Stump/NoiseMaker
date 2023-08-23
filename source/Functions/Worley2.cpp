#include "Worley2.hpp"


#include "../imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/random.hpp>


Worley2::Worley2()
	: distribution(0.0f, 1.0f)
{

}

float Worley2::sample(float x, float y)
{
	int cellX = glm::floor(x);
	int cellY = glm::floor(y);

	float minDistance	= 10.0f;
	float minDistance2	= 11.0f;

	for (int iy = -1; iy <= 1; iy++) {
		for (int ix = -1; ix <= 1; ix++) {
			glm::vec2 point = getPoint(cellX + ix, cellY + iy);

			float distance = glm::distance(point, { x, y });

			if (distance < minDistance) {
				minDistance2 = minDistance;
				minDistance = distance;
			}
			else if (minDistance < distance && distance < minDistance2) {
				minDistance2 = distance;
			}
		}
	}

	return minDistance2;
}

void Worley2::reseed(int seed)
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


glm::vec2 Worley2::getPoint(int cellX, int cellY)
{
	int ix = wrap(cellX, 256);
	int iy = wrap(cellY, 256);

	return glm::vec2(cellX, cellY) + points[iy][ix];
}