#ifndef FUNCTIONS_WORLEY_HPP
#define FUNCTIONS_WORLEY_HPP

#include <random>
#include <glm/glm.hpp>

#include "Function.hpp"

class Worley : public Function
{
public:
	Worley();

	float	sample(float x, float y);
	void	reseed(int seed);

protected:
	std::mt19937							engine;
	std::uniform_real_distribution<float>	distribution;

	glm::vec2	getPoint(int cellX, int cellY);

	glm::vec2 points[256][256];

};

#endif