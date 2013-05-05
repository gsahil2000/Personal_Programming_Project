/// Jai Mata Di
#pragma once
#include <math.h>
#include <float.h>

#define SINGLE_PRECISION 1

namespace RocketFrog
{
#if SINGLE_PRECISION
	typedef float number;

	/// @def num_sqrt
	/// easier to change later if we want to change the precision
#define num_sqrt sqrtf

#define num_pow powf

#define num_max	FLT_MAX

/// switch to double
#else if DOUBLE_PRECISION
	typedef double number;
	
#define num_sqrt	sqrt;
#define num_pow		pow;
#define num_max		DBL_MAX;
#endif
}