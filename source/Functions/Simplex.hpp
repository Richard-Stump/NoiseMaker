#ifndef FUNCTIONS_SIMPLEX_HPP
#define FUNCTIONS_SIMPLEX_HPP

#include "Function.hpp"

class Simplex : public Function
{
public:
	Simplex();

	float sample(float x, float y);
	void reseed(int seed);

protected:
	std::mt19937							engine;
	std::uniform_real_distribution<float>	distribution;

	glm::vec2	getGradient(int x, int y);

	glm::vec2 barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);
	float cross2d(glm::vec2 a, glm::vec2 b);

	glm::vec2 gradients[256][256];
};
#endif//FUNCTIONS_SIMPLEX_HPP