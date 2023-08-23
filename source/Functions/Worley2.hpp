#pragma once
#ifndef FUNCTIONS_WORLEY2_HPP
#define FUNCTIONS_WORLEY2_HPP

#include <random>
#include <glm/glm.hpp>

#include "Function.hpp"

class Worley2 : public Function
{
public:
	Worley2();

	float	sample(float x, float y);
	void	reseed(int seed);

protected:
	std::mt19937							engine;
	std::uniform_real_distribution<float>	distribution;

	glm::vec2	getPoint(int cellX, int cellY);

	glm::vec2 points[256][256];
};

#endif