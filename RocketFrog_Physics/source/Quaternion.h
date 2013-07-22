#pragma once
#include "Precision.h"
#include "Vector3.h"

/// @class Quaternion
/// Holds a three-degree-of-freedom orientation.
namespace RocketFrog
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(const number a_r, const number a_i, const number a_j, const number a_k);
		~Quaternion();

#pragma warning( push )
#pragma warning( disable : 4201 )
		union 
		{
			struct 
			{
				/* Holds the real component of the quaternion. */
				number r;

				/* first complex component of quaternion. */
				number i;

				/* second complex component of quaternion. */
				number j;

				/* third complex component of quaternion. */
				number k;
			};

			/* stores quaternion data in array form. */
			number m_data[4];
		};
#pragma warning ( pop )

		/* normalizes the quaternion. */
		void Normalize();

		/* multiplies this quaternion with given quaternion. */
		void operator*=(const Quaternion& a_rhs);

		/* rotate this quaternion by given vector */
		void RotateByVector(const Vector3& a_V);

		/* Updates by Angular Velocity.
		  Adds the given vector to this one, scaled by given amount.
		  This is used to update orientation quaternion by a rotation and time.
		*/
		void AddScaledVector(const Vector3& a_v, number a_nScale);
	};
}