#include "Simplex.hpp"

#include <vector>

#include "../imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

Simplex::Simplex()
	: distribution(0.0, 2 * glm::pi<float>())
{

}

float Simplex::sample(float x, float y)
{
	glm::vec2 origPoint(x, y);

	// Skew The points space
	glm::vec2 transPoint(x - 0.5f * y, y);

	glm::vec2 cell = glm::floor(transPoint);

	glm::vec2 testVec = (cell + glm::vec2(1, 1)) - cell;
	glm::vec2 toPoint = transPoint - (cell + glm::vec2(1, 0));
	float dp = glm::dot(toPoint, testVec);

	glm::vec2 v1, v2, v3;

	if (dp < 0.0) { // Lower Left
		v1 = cell;
		v2 = cell + glm::vec2(1, 0);
		v3 = cell + glm::vec2(0, 1);
	}
	else {	// Upper Right
		v3 = cell + glm::vec2(1, 0);	//v1
		v2 = cell + glm::vec2(1, 1);
		v1 = cell + glm::vec2(0, 1);
	}

	glm::vec2 transV1 = v1 + glm::vec2(0.5f * y, 0);
	glm::vec2 transV2 = v2 + glm::vec2(0.5f * y, 0);
	glm::vec2 transV3 = v3 + glm::vec2(0.5f * y, 0);

	glm::vec2 grad1 = getGradient(v1.x, v1.y);
	glm::vec2 grad2 = getGradient(v2.x, v2.y);
	glm::vec2 grad3 = getGradient(v3.x, v3.y);

	float dot1 = glm::dot(grad1, transPoint - transV1);
	float dot2 = glm::dot(grad2, transPoint - transV2);
	float dot3 = glm::dot(grad3, transPoint - transV3);

	glm::vec2 bar = barycentric(transPoint, transV1, transV2, transV3);

	float val = (1 - bar.x - bar.y) * dot1 +
		bar.x * dot2 +
		bar.y * dot3;

	return (val + 1) * 0.5f;
}

void Simplex::reseed(int seed)
{
	engine.seed(seed);

	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			float angle = distribution(engine);

			gradients[y][x] = glm::vec2(glm::cos(angle), glm::sin(angle));
		}
	}
}

glm::vec2 Simplex::getGradient(int cellX, int cellY)
{
	int ix = wrap(cellX, 256);
	int iy = wrap(cellY, 256);

	return gradients[iy][ix];
}

glm::vec2 Simplex::barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	float at = cross2d(b - a, c - a);

	float aa = cross2d(c - b, p - b) / at;
	float ab = cross2d(p - a, c - a) / at;

	return glm::vec3{ aa, ab, 1 - aa - ab };
}

float Simplex::cross2d(glm::vec2 a, glm::vec2 b)
{
	return a.x * b.y - a.y * b.x;
}