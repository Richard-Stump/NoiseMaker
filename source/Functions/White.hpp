#ifndef FUNCTIONS_WHITE_HPP
#define FUNCTIONS_WHITE_HPP

#include "Function.hpp"

#include <random>

class WhiteNoise : public Function
{
public:
	WhiteNoise();

	float sample(float x, float y);
	void reseed(int seed);

protected:
	std::mt19937							engine;
	std::uniform_real_distribution<float>	distribution;
	
	float preCalc[256][256];
};

#endif FUNCTIONS_WHITE_HPP