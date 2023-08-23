#ifndef FUNCTIONS_PERLIN_HPP
#define FUNCTIONS_PERLIN_HPP

#include "Function.hpp"

#include <random>
#include <glm/glm.hpp>

class Perlin : public Function
{
public:
	Perlin();

	float sample(float x, float y);
	void reseed(int seed);

protected:
	std::mt19937							engine;
	std::uniform_real_distribution<float>	distribution;

	glm::vec2	getGradient(int x, int y);
	float		dotGradient(float x, float y, int cellX, int cellY);

	glm::vec2 gradients[256][256];
};

#endif//FUNCTIONS_PERLIN_HPP