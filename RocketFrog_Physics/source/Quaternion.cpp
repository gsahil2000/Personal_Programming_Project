#include "Quaternion.h"

namespace RocketFrog
{
	Quaternion::Quaternion()
		:r(0), i(0), j(0), k(0)
	{}

	Quaternion::Quaternion(const number a_r, const number a_i, const number a_j, const number a_k)
		:r(a_r), i(a_i), j(a_j), k(a_k)
	{}

	Quaternion::~Quaternion()
	{}

	void Quaternion::Normalize()
	{
		number _d = r*r + i*i + j*j + k*k;

		// if zero-length quaternion, then use no-rotation.
		if (_d == 0)
		{
			r = 1;
			return;
		}

		_d = ((number)1.0)/num_sqrt(_d);
		r *= _d;
		i *= _d;
		j *= _d;
		k *= _d;
	}


	/*
		| r1 | | r2 | = | r1 r2  -  i1 i2  -  j1 j2  -  k1 k2 |
		| i1 | | i2 | = | r1 i2  +  i1 r2  +  j1 k2  -  k1 j2 |
		| j1 | | j2 | = | r1 j2  -  i1 k2  +  j1 r2  +  k1 i2 |
		| k1 | | k2 | = | r1 k2  +  i1 j2  -  j1 i2  +  k1 r2 |
	*/
	void Quaternion::operator*=(const Quaternion& a_rhs)
	{
		Quaternion _q = *this;
		
		r = _q.r * a_rhs.r - 
			_q.i * a_rhs.i -
			_q.j * a_rhs.j - 
			_q.k * a_rhs.k;

		i = _q.r * a_rhs.i +
			_q.i * a_rhs.r +
			_q.j * a_rhs.k + 
			_q.k * a_rhs.j;

		j = _q.r * a_rhs.j -
			_q.i * a_rhs.k +
			_q.j * a_rhs.r + 
			_q.k * a_rhs.i;

		k = _q.r * a_rhs.k +
			_q.i * a_rhs.j -
			_q.j * a_rhs.i + 
			_q.k * a_rhs.r;
	}

	void Quaternion::RotateByVector(const Vector3& a_v)
	{
		Quaternion _q(0, a_v.x, a_v.y, a_v.z);
		(*this) *= _q;
	}

	void Quaternion::AddScaledVector(const Vector3& a_v, const number a_nScale)
	{
		Quaternion _q(0, a_v.x * a_nScale, a_v.y * a_nScale, a_v.z * a_nScale);
		_q *= *this;
		r += _q.r * ((number)0.5);
		i += _q.i * ((number)0.5);
		j += _q.j * ((number)0.5);
		k += _q.k * ((number)0.5);
	}
}