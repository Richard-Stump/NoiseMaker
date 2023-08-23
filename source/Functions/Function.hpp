#ifndef FUNCTIONS_FUNCTION_HPP
#define FUNCTIONS_FUNCTION_HPP

#include <random>

class Function
{
public:
	virtual float	sample(float x, float y) = 0;
	virtual void	reseed(int seed) = 0;
};

inline float fade(float t) 
{
	return 3 * t * t - 2 * t * t * t;
}

inline int wrap(int index, int size)
{
	return ((index % size) + size) % size;
}

#include "Perlin.hpp"
#include "White.hpp"
#include "Worley.hpp"
#include "Value.hpp"
#include "Simplex.hpp"
#include "Worley2.hpp"

#endif//FUNCTIONS_FUNCTION_HPP