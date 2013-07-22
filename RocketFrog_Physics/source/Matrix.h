#pragma once
#include "Precision.h"
#include "Vector3.h"
#include "Quaternion.h"

/// @class Matrix3x3
/// Holds a 3 x 3 row major matrix representing a transformation in 
/// 3D space that does not include translational component.

namespace RocketFrog
{
	class Matrix3x3
	{
	public:
		Matrix3x3();
		
		Matrix3x3(const Matrix3x3& a_mRhs);

		Matrix3x3(const number a_00, const number a_01, const number a_02,
			const number a_03, const number a_04, const number a_05,
			const number a_06, const number a_07, const number a_08);

		Matrix3x3(const number a_mData[9]);

		/* Holds the matrix numbers in array form. */
		number m_data[9];
	
		/*
			returns Vector3 = Matrix x Vector3;
			| a b c | | x |   | ax + by + cz |
			| d e f | | y | = | dx + ey + fz |
			| g h i | | z |   | gx + hy + iz |
		*/
		Vector3 operator*(const Vector3& a_vRhs) const;

		/* Transforms the given Vector3 by this matrix. */
		Vector3 Transform(const Vector3& a_vRhs) const;

		/* returns Matrix = this * givenMatix; */
		Matrix3x3 operator*(const Matrix3x3& a_mRhs) const;

		/* in place matrix multiplication with the given matrix. */
		void operator*=(const Matrix3x3& a_mRhs);

		/* sets the matrix to be the inverse of the given matrix. */
		void SetInverse(const Matrix3x3& a_mOut);

		/* returns a new matrix containing inverse of this matrix. */
		Matrix3x3 Inverse() const;

		/* Inverts the matrix. */
		void Invert();

		/* sets matrix to transpose of given matrix. */
		void SetTranspose(const Matrix3x3& a_mMatrix);

		/* Sets this matrix to be the rotation matrix 
		   corresponding to the given quaternion. 
		 */
		void SetOrientation(const Quaternion& _q);

		/* returns transpose of this matrix. */
		Matrix3x3 Transpose() const;

		/* Sets the value of the matrix as an inertia tensor of
           a rectangular block aligned with the body's coordinate
           system with the given axis half-sizes and mass. */
        void SetBlockInertiaTensor(const Vector3& a_vHalfSizes, const number a_nMass);

		/* Sets the value of the matrix from inertia tensor values. */
        void SetInertiaTensorCoeffs(number ix, number iy, number iz,
            number ixy=0, number ixz=0, number iyz=0);


		/* Interpolates a couple of matrices. */
		static Matrix3x3 LinearInterpolate(const Matrix3x3& a, const Matrix3x3& b, const number prop);

	private:
	};
	
	
	/// @class Matrix4x4
	/// Holds a 3 x 4 row major transformation matrix.
	class Matrix3x4
	{
	public:
		number m_data[12];

		/*
			returns Vector3 = Matrix x vector4(vector3.w = 1);
			| a b c d | | x |   | ax + by + cz + d |
			| e f g h | | y | = | ex + fy + gz + h |
			| i j k l | | z |   | ix + jy + kz + l |
			            | 1 |
		*/
		Vector3 operator*(const Vector3& a_vRhs) const;

		/* Transforms the given Vector3 by this matrix. */
		Vector3 Transform(const Vector3& a_vRhs) const;

		/* returns a new matrix which is the product of this and given matrix. */
		Matrix3x4 operator*(const Matrix3x4& a_mRhs) const;

		/* returns the determinant of this matrix. */
		number GetDeterminant() const;

		/* sets given matrix to be inverse of this matrix. */
		void SetInverse(const Matrix3x4& a_mOut);

		/* returns inverse of this matrix. */
		Matrix3x4 Inverse() const;

		/* Inverts the matrix. */
		void Invert();

		/*  */
		Vector3 LocalToWorld(const Vector3& a_vLocal, const Matrix3x4& a_mTransform);

		/* Sets this matrix to be the rotation matrix 
		   corresponding to the given quaternion. 
		 */
		void SetOrientationAndPosition(const Quaternion& _q, const Vector3& a_vPos);

		/* Transform the given vector by transformational ivnerse of this matrix. */
		Vector3 TransformInverse(const Vector3& a_v) const;

		/* Transform the given direction vector by this matrix. */
		Vector3 TransformDirection(const Vector3& a_v) const;

		/* Transform the given direction vector by transformational inverse of this matrix. */
		Vector3 TransformInverseDirection(const Vector3& a_v) const;

		void FillGLArray(float a_data[16]) const;

	private:

	};
}