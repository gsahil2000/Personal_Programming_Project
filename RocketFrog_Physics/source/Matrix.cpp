#include "Matrix.h"

namespace RocketFrog
{
#pragma region Matrix3x3
	Matrix3x3::Matrix3x3()
		//:m_data()
	{
		for (int i = 0; i < 9; ++i)
		{
			m_data[i] = 0;
		}
	}

	Matrix3x3::Matrix3x3(const Matrix3x3& a_mRhs)
		//:m_data()
	{
		for (unsigned short i = 0; i < 9; ++i)
		{
			m_data[i] = a_mRhs.m_data[i];
		}
	}

	Matrix3x3::Matrix3x3(const number a_00, const number a_01, const number a_02,
			const number a_03, const number a_04, const number a_05,
			const number a_06, const number a_07, const number a_08)
			//:m_data()
	{
		m_data[0] = a_00;
		m_data[1] = a_01;
		m_data[2] = a_02;

		m_data[3] = a_03;
		m_data[4] = a_04;
		m_data[5] = a_05;

		m_data[6] = a_06;
		m_data[7] = a_07;
		m_data[8] = a_08;
	}

	Matrix3x3::Matrix3x3(const number a_mData[9])
		//:m_data()
	{
		for (unsigned int i = 0; i < 9; ++i)
		{
			m_data[i] = a_mData[i];
		}
	}

	Vector3 Matrix3x3::operator*(const Vector3& a_vRhs) const
	{
		return Vector3( ( (m_data[0] * a_vRhs.x) + (m_data[1] * a_vRhs.y) + (m_data[2] * a_vRhs.z) ),
			( (m_data[3] * a_vRhs.x ) + ( m_data[4] * a_vRhs.y ) + ( m_data[5] * a_vRhs.z) ),
			( (m_data[6] * a_vRhs.x ) + ( m_data[7] * a_vRhs.y ) + ( m_data[8] * a_vRhs.z) ) );
	}

	Vector3 Matrix3x3::Transform(const Vector3& a_vRhs) const
	{
		return (*this) * a_vRhs;
	}

	Matrix3x3 Matrix3x3::operator*(const Matrix3x3& a_mRhs) const
	{
		return Matrix3x3(
			m_data[0]*a_mRhs.m_data[0] + m_data[1]*a_mRhs.m_data[3] + m_data[2]*a_mRhs.m_data[6],
			m_data[0]*a_mRhs.m_data[1] + m_data[1]*a_mRhs.m_data[4] + m_data[2]*a_mRhs.m_data[7],
			m_data[0]*a_mRhs.m_data[2] + m_data[1]*a_mRhs.m_data[5] + m_data[2]*a_mRhs.m_data[8],

			m_data[3]*a_mRhs.m_data[0] + m_data[4]*a_mRhs.m_data[3] + m_data[5]*a_mRhs.m_data[6],
			m_data[3]*a_mRhs.m_data[1] + m_data[4]*a_mRhs.m_data[4] + m_data[5]*a_mRhs.m_data[7],
			m_data[3]*a_mRhs.m_data[2] + m_data[4]*a_mRhs.m_data[5] + m_data[5]*a_mRhs.m_data[8],

			m_data[6]*a_mRhs.m_data[0] + m_data[7]*a_mRhs.m_data[3] + m_data[8]*a_mRhs.m_data[6],
			m_data[6]*a_mRhs.m_data[1] + m_data[7]*a_mRhs.m_data[4] + m_data[8]*a_mRhs.m_data[7],
			m_data[6]*a_mRhs.m_data[2] + m_data[7]*a_mRhs.m_data[5] + m_data[8]*a_mRhs.m_data[8]
		);
	}

	void Matrix3x3::operator*=(const Matrix3x3& a_mRhs)
	{
		m_data[0] = m_data[0]*a_mRhs.m_data[0] + m_data[1]*a_mRhs.m_data[3] + m_data[2]*a_mRhs.m_data[6];
		m_data[1] = m_data[0]*a_mRhs.m_data[1] + m_data[1]*a_mRhs.m_data[4] + m_data[2]*a_mRhs.m_data[7];
		m_data[2] = m_data[0]*a_mRhs.m_data[2] + m_data[1]*a_mRhs.m_data[5] + m_data[2]*a_mRhs.m_data[8];

		m_data[3] = m_data[3]*a_mRhs.m_data[0] + m_data[4]*a_mRhs.m_data[3] + m_data[5]*a_mRhs.m_data[6];
		m_data[4] = m_data[3]*a_mRhs.m_data[1] + m_data[4]*a_mRhs.m_data[4] + m_data[5]*a_mRhs.m_data[7];
		m_data[5] = m_data[3]*a_mRhs.m_data[2] + m_data[4]*a_mRhs.m_data[5] + m_data[5]*a_mRhs.m_data[8];

		m_data[6] = m_data[6]*a_mRhs.m_data[0] + m_data[7]*a_mRhs.m_data[3] + m_data[8]*a_mRhs.m_data[6];
		m_data[7] = m_data[6]*a_mRhs.m_data[1] + m_data[7]*a_mRhs.m_data[4] + m_data[8]*a_mRhs.m_data[7];
		m_data[8] = m_data[6]*a_mRhs.m_data[2] + m_data[7]*a_mRhs.m_data[5] + m_data[8]*a_mRhs.m_data[8];
	}

	void Matrix3x3::SetInverse(const Matrix3x3& a_mOut)
	{
		number _temp[6];
		_temp[0] = a_mOut.m_data[0] * a_mOut.m_data[4];
		_temp[1] = a_mOut.m_data[0] * a_mOut.m_data[5];
		_temp[2] = a_mOut.m_data[1] * a_mOut.m_data[3];
		_temp[3] = a_mOut.m_data[2] * a_mOut.m_data[3];
		_temp[4] = a_mOut.m_data[1] * a_mOut.m_data[6];
		_temp[5] = a_mOut.m_data[2] * a_mOut.m_data[6];

		// calculate determinant.
		number _determinant = (_temp[0]*a_mOut.m_data[8] - _temp[1]*a_mOut.m_data[7] - _temp[2]*a_mOut.m_data[8] +
							   _temp[3]*a_mOut.m_data[7] + _temp[4]*a_mOut.m_data[5] - _temp[5]*a_mOut.m_data[4] );

		if (_determinant == 0.0f)
			return;

		number _invd = (number)1.0 / _determinant;

		m_data[0] = _invd *  (a_mOut.m_data[4] * a_mOut.m_data[8]  -  a_mOut.m_data[5] * a_mOut.m_data[7]);
		m_data[1] = _invd * -(a_mOut.m_data[1] * a_mOut.m_data[8]  -  a_mOut.m_data[2] * a_mOut.m_data[7]);
		m_data[2] = _invd *  (a_mOut.m_data[1] * a_mOut.m_data[5]  -  a_mOut.m_data[2] * a_mOut.m_data[4]);
		m_data[3] = _invd * -(a_mOut.m_data[3] * a_mOut.m_data[8]  -  a_mOut.m_data[5] * a_mOut.m_data[6]);
		m_data[4] = _invd *  (a_mOut.m_data[0] * a_mOut.m_data[8]  -  _temp[5]);
		m_data[5] = _invd * -(_temp[2] - _temp[4]);
		m_data[6] = _invd *  (a_mOut.m_data[3] * a_mOut.m_data[7]  -  a_mOut.m_data[4] * a_mOut.m_data[6]);
		m_data[7] = _invd * -(a_mOut.m_data[0] * a_mOut.m_data[7]  -  _temp[4]);
		m_data[8] = _invd *  (_temp[0] - _temp[2]);
	}

	Matrix3x3 Matrix3x3::Inverse() const
	{
		Matrix3x3 _result;
		_result.SetInverse(*this);
		return _result;
	}

	void Matrix3x3::Invert()
	{
		SetInverse(*this);
	}

	void Matrix3x3::SetTranspose(const Matrix3x3& a_mMatrix)
	{
		m_data[0] = a_mMatrix.m_data[0];
		m_data[1] = a_mMatrix.m_data[3];
		m_data[2] = a_mMatrix.m_data[6];
		m_data[3] = a_mMatrix.m_data[1];
		m_data[4] = a_mMatrix.m_data[4];
		m_data[5] = a_mMatrix.m_data[7];
		m_data[6] = a_mMatrix.m_data[2];
		m_data[7] = a_mMatrix.m_data[5];
		m_data[8] = a_mMatrix.m_data[8];
	}

	void Matrix3x3::SetOrientation(const Quaternion& _q)
	{
		m_data[0] = 1 - (2*_q.j*_q.j + 2*_q.k*_q.k);
		m_data[1] = 2*_q.i*_q.j + 2*_q.k*_q.r;
		m_data[2] = 2*_q.i*_q.k - 2*_q.j*_q.r;
		m_data[3] = 2*_q.i*_q.j - 2*_q.k*_q.r;
		m_data[4] = 1 - (2*_q.i*_q.i  + 2*_q.k*_q.k);
		m_data[5] = 2*_q.j*_q.k + 2*_q.i*_q.r;
		m_data[6] = 2*_q.i*_q.k + 2*_q.j*_q.r;
		m_data[7] = 2*_q.j*_q.k - 2*_q.i*_q.r;
		m_data[8] = 1 - (2*_q.i*_q.i  + 2*_q.j*_q.j);
	}

	Matrix3x3 Matrix3x3::Transpose() const
	{
		Matrix3x3 _result;
		_result.SetTranspose(*this);
		return _result;
	}

	Matrix3x3 Matrix3x3::LinearInterpolate(const Matrix3x3& a, const Matrix3x3& b, const number prop)
	{
		Matrix3x3 _result;
		number _omp = number(1.0) - prop;
		for (unsigned i = 0; i < 9; ++i)
		{
			_result.m_data[i] = a.m_data[i] * _omp + b.m_data[i] * prop;
		}
		return _result;
	}

	void Matrix3x3::SetBlockInertiaTensor(const Vector3& a_vHalfSizes, const number a_nMass)
	{
		Vector3 squares = a_vHalfSizes * a_vHalfSizes; // multiplies each component with each other. Squaring.
        SetInertiaTensorCoeffs(0.3f*a_nMass*(squares.y + squares.z),
            0.3f*a_nMass*(squares.x + squares.z),
            0.3f*a_nMass*(squares.x + squares.y));
	}

	void Matrix3x3::SetInertiaTensorCoeffs(number ix, number iy, number iz,
            number ixy, number ixz, number iyz)
	{
		m_data[0] = ix;
        m_data[1] = m_data[3] = -ixy;
        m_data[2] = m_data[6] = -ixz;
        m_data[4] = iy;
        m_data[5] = m_data[7] = -iyz;
        m_data[8] = iz;
	}
#pragma endregion //Matrix3x3

#pragma region Matrix3x4
	Vector3 Matrix3x4::operator*(const Vector3& a_vRhs) const
	{
		return Vector3( 
			m_data[0]  * a_vRhs.x + 
			m_data[1]  * a_vRhs.y + 
			m_data[2]  * a_vRhs.z + m_data[3],
					   
			m_data[4]  * a_vRhs.x + 
			m_data[5]  * a_vRhs.y +
			m_data[6]  * a_vRhs.z + m_data[7],
					   
			m_data[8]  * a_vRhs.x + 
			m_data[9]  * a_vRhs.y + 
			m_data[10] * a_vRhs.z + m_data[11]
		);
	}

	Vector3 Matrix3x4::Transform(const Vector3& a_vRhs) const
	{
		return this->operator*(a_vRhs);
	}

	Matrix3x4 Matrix3x4::operator*(const Matrix3x4& a_mRhs) const
	{
		Matrix3x4 _returnValue;

		_returnValue.m_data[0]  = m_data[0]*a_mRhs.m_data[0] + m_data[1]*a_mRhs.m_data[4] + m_data[2]*a_mRhs.m_data[8];
		_returnValue.m_data[1]  = m_data[0]*a_mRhs.m_data[1] + m_data[1]*a_mRhs.m_data[5] + m_data[2]*a_mRhs.m_data[9];
		_returnValue.m_data[2]  = m_data[0]*a_mRhs.m_data[2] + m_data[1]*a_mRhs.m_data[6] + m_data[2]*a_mRhs.m_data[10];
		_returnValue.m_data[3]  = m_data[0]*a_mRhs.m_data[3] + m_data[1]*a_mRhs.m_data[7] + m_data[2]*a_mRhs.m_data[11] + m_data[3];
							    
		_returnValue.m_data[4]  = m_data[4]*a_mRhs.m_data[0] + m_data[5]*a_mRhs.m_data[4] + m_data[6]*a_mRhs.m_data[8];
		_returnValue.m_data[5]  = m_data[4]*a_mRhs.m_data[1] + m_data[5]*a_mRhs.m_data[5] + m_data[6]*a_mRhs.m_data[9];
		_returnValue.m_data[6]  = m_data[4]*a_mRhs.m_data[2] + m_data[5]*a_mRhs.m_data[6] + m_data[6]*a_mRhs.m_data[10];
		_returnValue.m_data[7]  = m_data[4]*a_mRhs.m_data[3] + m_data[5]*a_mRhs.m_data[7] + m_data[6]*a_mRhs.m_data[11] + m_data[7];
							    
		_returnValue.m_data[8]  = m_data[8]*a_mRhs.m_data[0] + m_data[9]*a_mRhs.m_data[4] + m_data[10]*a_mRhs.m_data[8];
		_returnValue.m_data[9]  = m_data[8]*a_mRhs.m_data[1] + m_data[9]*a_mRhs.m_data[5] + m_data[10]*a_mRhs.m_data[9];
		_returnValue.m_data[10] = m_data[8]*a_mRhs.m_data[2] + m_data[9]*a_mRhs.m_data[6] + m_data[10]*a_mRhs.m_data[10];
		_returnValue.m_data[11] = m_data[8]*a_mRhs.m_data[3] + m_data[9]*a_mRhs.m_data[7] + m_data[10]*a_mRhs.m_data[11] + m_data[11];

		return _returnValue;
	}

	number Matrix3x4::GetDeterminant() const
	{
		return m_data[8] * m_data[5] * m_data[2]  +
			   m_data[4] * m_data[9] * m_data[2]  +
			   m_data[8] * m_data[1] * m_data[6]  -
			   m_data[0] * m_data[9] * m_data[6]  -
			   m_data[4] * m_data[1] * m_data[10] +
			   m_data[0] * m_data[5] * m_data[10];
	}

	void Matrix3x4::SetInverse(const Matrix3x4& a_mOut)
	{
		// return if determinant = 0;
		number _determinant = GetDeterminant();
		if (_determinant == 0)
			return;

		_determinant = (number)1.0f / _determinant;

		m_data[0]  = _determinant * (-a_mOut.m_data[9] * a_mOut.m_data[6]  +  a_mOut.m_data[5] * a_mOut.m_data[10] );
		m_data[1]  = _determinant * ( a_mOut.m_data[9] * a_mOut.m_data[2]  -  a_mOut.m_data[1] * a_mOut.m_data[10] );
		m_data[2]  = _determinant * (-a_mOut.m_data[5] * a_mOut.m_data[2]  +  a_mOut.m_data[1] * a_mOut.m_data[6] * a_mOut.m_data[15] );
		m_data[3]  = _determinant * ( a_mOut.m_data[9] * a_mOut.m_data[6]  * a_mOut.m_data[3]  -
									  a_mOut.m_data[5] * a_mOut.m_data[10] * a_mOut.m_data[3]  -
									  a_mOut.m_data[9] * a_mOut.m_data[2]  * a_mOut.m_data[7]  +
									  a_mOut.m_data[1] * a_mOut.m_data[10] * a_mOut.m_data[7]  +
									  a_mOut.m_data[5] * a_mOut.m_data[2]  * a_mOut.m_data[11] -
									  a_mOut.m_data[1] * a_mOut.m_data[6]  * a_mOut.m_data[11] );

		m_data[4]  = _determinant * ( a_mOut.m_data[8] * a_mOut.m_data[6]  -  a_mOut.m_data[4] * a_mOut.m_data[10] );
		m_data[5]  = _determinant * (-a_mOut.m_data[8] * a_mOut.m_data[2]  +  a_mOut.m_data[0] * a_mOut.m_data[10] );
		m_data[6]  = _determinant * ( a_mOut.m_data[4] * a_mOut.m_data[2]  -  a_mOut.m_data[0] * a_mOut.m_data[6] * a_mOut.m_data[15] );
		m_data[7]  = _determinant * (-a_mOut.m_data[8] * a_mOut.m_data[6]  * a_mOut.m_data[3]  +
									  a_mOut.m_data[4] * a_mOut.m_data[10] * a_mOut.m_data[3]  +
									  a_mOut.m_data[8] * a_mOut.m_data[2]  * a_mOut.m_data[7]  -
									  a_mOut.m_data[0] * a_mOut.m_data[10] * a_mOut.m_data[7]  -
									  a_mOut.m_data[4] * a_mOut.m_data[2]  * a_mOut.m_data[11] +
									  a_mOut.m_data[0] * a_mOut.m_data[6]  * a_mOut.m_data[11] );

		m_data[8]  = _determinant * (-a_mOut.m_data[8] * a_mOut.m_data[5]  +  a_mOut.m_data[4] * a_mOut.m_data[9] * a_mOut.m_data[15] );
		m_data[9]  = _determinant * ( a_mOut.m_data[8] * a_mOut.m_data[1]  -  a_mOut.m_data[0] * a_mOut.m_data[9] * a_mOut.m_data[15] );
		m_data[10] = _determinant * (-a_mOut.m_data[4] * a_mOut.m_data[1]  +  a_mOut.m_data[0] * a_mOut.m_data[5] * a_mOut.m_data[15] );
		m_data[11] = _determinant * ( a_mOut.m_data[8] * a_mOut.m_data[5]  * a_mOut.m_data[3]  -
									  a_mOut.m_data[4] * a_mOut.m_data[9]  * a_mOut.m_data[3]  -
									  a_mOut.m_data[8] * a_mOut.m_data[1]  * a_mOut.m_data[7]  +
									  a_mOut.m_data[0] * a_mOut.m_data[9]  * a_mOut.m_data[7]  +
									  a_mOut.m_data[4] * a_mOut.m_data[1]  * a_mOut.m_data[11] -
									  a_mOut.m_data[0] * a_mOut.m_data[5]  * a_mOut.m_data[11] );

	}

	Matrix3x4 Matrix3x4::Inverse() const
	{
		Matrix3x4 _result;
		_result.SetInverse(*this);
		return _result;
	}

	void Matrix3x4::Invert()
	{
		this->SetInverse(*this);
	}

	void Matrix3x4::SetOrientationAndPosition(const Quaternion& _q, const Vector3& a_vPos)
	{
		m_data[0]  = 1 - (2*_q.j*_q.j + 2*_q.k*_q.k);
        m_data[1]  = 2*_q.i*_q.j + 2*_q.k*_q.r;
        m_data[2]  = 2*_q.i*_q.k - 2*_q.j*_q.r;
        m_data[3]  = a_vPos.x;
				   
        m_data[4]  = 2*_q.i*_q.j - 2*_q.k*_q.r;
        m_data[5]  = 1 - (2*_q.i*_q.i  + 2*_q.k*_q.k);
        m_data[6]  = 2*_q.j*_q.k + 2*_q.i*_q.r;
        m_data[7]  = a_vPos.y;
				   
        m_data[8]  = 2*_q.i*_q.k + 2*_q.j*_q.r;
        m_data[9]  = 2*_q.j*_q.k - 2*_q.i*_q.r;
        m_data[10] = 1 - (2*_q.i*_q.i  + 2*_q.j*_q.j);
        m_data[11] = a_vPos.z;
	}

	Vector3 Matrix3x4::TransformInverse(const Vector3& a_v) const
	{
		Vector3 _temp = a_v;
		_temp.x -= m_data[3];
		_temp.y -= m_data[7];
		_temp.z -= m_data[13];

		return Vector3(
			_temp.x * m_data[0] + _temp.y * m_data[4] + _temp.z * m_data[8],
			_temp.x * m_data[1] + _temp.y * m_data[5] + _temp.z * m_data[9],
			_temp.x * m_data[2] + _temp.y * m_data[6] + _temp.z * m_data[10]
		);
	}

	Vector3 Matrix3x4::TransformDirection(const Vector3& a_v) const
	{
		return Vector3(
                a_v.x * m_data[0] +
                a_v.y * m_data[1] +
                a_v.z * m_data[2],

                a_v.x * m_data[4] +
                a_v.y * m_data[5] +
                a_v.z * m_data[6],

                a_v.x * m_data[8] +
                a_v.y * m_data[9] +
                a_v.z * m_data[10]
            );
	}

	Vector3 Matrix3x4::TransformInverseDirection(const Vector3& a_v) const
	{
		return Vector3(
                a_v.x * m_data[0] +
                a_v.y * m_data[4] +
                a_v.z * m_data[8],

                a_v.x * m_data[1] +
                a_v.y * m_data[5] +
                a_v.z * m_data[9],

                a_v.x * m_data[2] +
                a_v.y * m_data[6] +
                a_v.z * m_data[10]
            );
	}

	void Matrix3x4::FillGLArray(float a_fData[16]) const
	{
		a_fData[0]  = (float)m_data[0];
		a_fData[1]  = (float)m_data[4];
		a_fData[2]  = (float)m_data[8];
		a_fData[3]  = (float)0;
				      
		a_fData[4]  = (float)m_data[1];
		a_fData[5]  = (float)m_data[5];
		a_fData[6]  = (float)m_data[9];
		a_fData[7]  = (float)0;
				      
		a_fData[8]  = (float)m_data[2];
		a_fData[9]  = (float)m_data[6];
		a_fData[10] = (float)m_data[10];
		a_fData[11] = (float)0;
					  
		a_fData[12] = (float)m_data[3];
		a_fData[13] = (float)m_data[7];
		a_fData[14] = (float)m_data[11];
		a_fData[15] = (float)1.0f;
	}
#pragma endregion //Matrix3x4
}