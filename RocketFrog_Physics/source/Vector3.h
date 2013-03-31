#pragma once
#include "Precision.h"

/// @file Vector.h
/// Declaration of Vector3 class.

namespace RocketFrog
{
	/// @class Vector3
	/// Stores 3 dimensional value. Holds four members to align memory and make it efficient.
	class Vector3
	{
	public:
		/// default contructor, assigns all values to 0.
		Vector3();

		/// Ctor - assign given values to members.
		Vector3(const float_t a_x,const float_t a_y,const float_t a_z);

		/// Ctor - assign the scalar value to all three members.
		Vector3(const float_t a_all);

		/// Copy Ctor.
		Vector3(const Vector3& a_rhs);

		/// Assignment Operator.
		Vector3& operator=(const Vector3 &a_rhs);

		/// Dtor.
		~Vector3();

		/// check for equality
		bool operator==(const Vector3& a_rhs) const;

		/// check if not-equal
		bool operator!=(const Vector3& a_rhs) const;

		/// greater than
		bool operator>(const Vector3& a_rhs) const;

		/// greater than or equal
		bool operator>=(const Vector3& a_rhs) const;

		/// less than
		bool operator<(const Vector3& a_rhs) const;

		/// less than or equal
		bool operator<=(const Vector3& a_rhs) const;

		/// Multiplies all members with -1.
		void Invert();

		/// addition operator
		Vector3 operator+(const Vector3& a_rhs) const;

		/// add and assign
		void operator+=(const Vector3& a_rhs);

		/// operator-
		Vector3 operator-(const Vector3& a_rhs) const;

		/// operator-=
		void operator-=(const Vector3& a_rhs);

		/// operator*
		Vector3 operator*(const float_t a_scalar) const;

		/// component product
		Vector3 operator*(const Vector3& a_rhs) const;

		/// operator*= with scalar
		void operator*=(const float_t a_scalar);

		/// operator*= with vector
		void operator*=(const Vector3& a_rhs);

		/// Multiplies the vector with given scalar value and add to self
		void AddScaledVector(const Vector3& a_vector, const float_t a_scale);

		/// Returns Magnitude of the vector
		float_t Magnitude() const;

		/// returns the squared magnitude of the vector(more efficient).
		float_t SquaredMagnitude() const;

		/// turns a non-zero vector into unit vector
		void Normalize();

		/// returns the dot product(scalar product) of this vector with given vector
		float_t DotProduct(const Vector3& a_rhs) const;

		/// returns the cross product(vector product) of this vector with give vector
		Vector3 CrossProduct(const Vector3& a_rhs) const;

		/// calculates cross prouct of this vector with given vector and assigns to this vector
		void CrossProductUpdate(const Vector3& a_rhs);

		/// set all values to 0
		void Clear();

		/// members to hold value of 3 dimensions. Public to give quick access to users.
		float_t x, y, z;
		
	private:
		float_t padding;
	};
}