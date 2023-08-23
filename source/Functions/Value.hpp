#ifndef FUNCTIONS_VALUE_HPP
#define FUNCTIONS_VALUE_HPP

#include <vector>

#include "Function.hpp"

class Value : public Function
{
public:
	Value();

	float	sample(float x, float y);
	void	reseed(int seed);

protected:
	std::mt19937							engine;
	std::uniform_real_distribution<float>	distribution;

	float getPrecalcValue(int x, int y);

	float preCalc[256][256];
};

#endif//FUNCTIONS_VALUE_HPP