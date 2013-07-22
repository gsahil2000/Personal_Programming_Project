#include "Rigidbody.h"
#include <assert.h>
#include <memory>

namespace RocketFrog
{
	Rigidbody::Rigidbody()
		:m_nInverseMass(0), m_nLinearDamping(0), m_vPosition(0), m_qOrientation(0,0,0,0),
		m_vVelocity(0), m_vRotation(0), m_mTransformMatrix()
	{}

	Rigidbody::~Rigidbody()
	{}

	/* Internal function to do an intertia tensor transform by a quaternion.
	  Note that the implementation of this function was created by an
	  automated code-generator and optimizer. */
	static inline void _transformInertiaTensor(Matrix3x3 &iitWorld,
											   //const Quaternion &q,
											   const Matrix3x3 &iitBody,
											   const Matrix3x4 &rotmat)
	{
		number t4 = rotmat.m_data[0]*iitBody.m_data[0]+
			rotmat.m_data[1]*iitBody.m_data[3]+
			rotmat.m_data[2]*iitBody.m_data[6];
		number t9 = rotmat.m_data[0]*iitBody.m_data[1]+
			rotmat.m_data[1]*iitBody.m_data[4]+
			rotmat.m_data[2]*iitBody.m_data[7];
		number t14 = rotmat.m_data[0]*iitBody.m_data[2]+
			rotmat.m_data[1]*iitBody.m_data[5]+
			rotmat.m_data[2]*iitBody.m_data[8];
		number t28 = rotmat.m_data[4]*iitBody.m_data[0]+
			rotmat.m_data[5]*iitBody.m_data[3]+
			rotmat.m_data[6]*iitBody.m_data[6];
		number t33 = rotmat.m_data[4]*iitBody.m_data[1]+
			rotmat.m_data[5]*iitBody.m_data[4]+
			rotmat.m_data[6]*iitBody.m_data[7];
		number t38 = rotmat.m_data[4]*iitBody.m_data[2]+
			rotmat.m_data[5]*iitBody.m_data[5]+
			rotmat.m_data[6]*iitBody.m_data[8];
		number t52 = rotmat.m_data[8]*iitBody.m_data[0]+
			rotmat.m_data[9]*iitBody.m_data[3]+
			rotmat.m_data[10]*iitBody.m_data[6];
		number t57 = rotmat.m_data[8]*iitBody.m_data[1]+
			rotmat.m_data[9]*iitBody.m_data[4]+
			rotmat.m_data[10]*iitBody.m_data[7];
		number t62 = rotmat.m_data[8]*iitBody.m_data[2]+
			rotmat.m_data[9]*iitBody.m_data[5]+
			rotmat.m_data[10]*iitBody.m_data[8];

		iitWorld.m_data[0] = t4*rotmat.m_data[0]+
			t9*rotmat.m_data[1]+
			t14*rotmat.m_data[2];
		iitWorld.m_data[1] = t4*rotmat.m_data[4]+
			t9*rotmat.m_data[5]+
			t14*rotmat.m_data[6];
		iitWorld.m_data[2] = t4*rotmat.m_data[8]+
			t9*rotmat.m_data[9]+
			t14*rotmat.m_data[10];
		iitWorld.m_data[3] = t28*rotmat.m_data[0]+
			t33*rotmat.m_data[1]+
			t38*rotmat.m_data[2];
		iitWorld.m_data[4] = t28*rotmat.m_data[4]+
			t33*rotmat.m_data[5]+
			t38*rotmat.m_data[6];
		iitWorld.m_data[5] = t28*rotmat.m_data[8]+
			t33*rotmat.m_data[9]+
			t38*rotmat.m_data[10];
		iitWorld.m_data[6] = t52*rotmat.m_data[0]+
			t57*rotmat.m_data[1]+
			t62*rotmat.m_data[2];
		iitWorld.m_data[7] = t52*rotmat.m_data[4]+
			t57*rotmat.m_data[5]+
			t62*rotmat.m_data[6];
		iitWorld.m_data[8] = t52*rotmat.m_data[8]+
			t57*rotmat.m_data[9]+
			t62*rotmat.m_data[10];
	}

	static inline void _CalculateTransformMatrix(Matrix3x4& a_transformMatrix, 
									const Vector3& a_vPos, const Quaternion& a_qOrientation)
	{
		a_transformMatrix.m_data[0] = 1-2*a_qOrientation.j*a_qOrientation.j-
        2*a_qOrientation.k*a_qOrientation.k;
		a_transformMatrix.m_data[1] = 2*a_qOrientation.i*a_qOrientation.j -
			2*a_qOrientation.r*a_qOrientation.k;
		a_transformMatrix.m_data[2] = 2*a_qOrientation.i*a_qOrientation.k +
			2*a_qOrientation.r*a_qOrientation.j;
		a_transformMatrix.m_data[3] = a_vPos.x;

		a_transformMatrix.m_data[4] = 2*a_qOrientation.i*a_qOrientation.j +
			2*a_qOrientation.r*a_qOrientation.k;
		a_transformMatrix.m_data[5] = 1-2*a_qOrientation.i*a_qOrientation.i-
			2*a_qOrientation.k*a_qOrientation.k;
		a_transformMatrix.m_data[6] = 2*a_qOrientation.j*a_qOrientation.k -
			2*a_qOrientation.r*a_qOrientation.i;
		a_transformMatrix.m_data[7] = a_vPos.y;

		a_transformMatrix.m_data[8] = 2*a_qOrientation.i*a_qOrientation.k -
			2*a_qOrientation.r*a_qOrientation.j;
		a_transformMatrix.m_data[9] = 2*a_qOrientation.j*a_qOrientation.k +
			2*a_qOrientation.r*a_qOrientation.i;
		a_transformMatrix.m_data[10] = 1-2*a_qOrientation.i*a_qOrientation.i-
			2*a_qOrientation.j*a_qOrientation.j;
		a_transformMatrix.m_data[11] = a_vPos.z;
	}

	void Rigidbody::CalculateDerivedData()
	{
		m_qOrientation.Normalize();

		// calculate the transform matrix for the body.
		_CalculateTransformMatrix(m_mTransformMatrix, m_vPosition, m_qOrientation);

		// calculate the inertiaTensor in world space.
		_transformInertiaTensor(m_mInverseInertiaTensorWorld, //m_qOrientation, 
							m_mInverseInertiaTensor, m_mTransformMatrix);
	}

	void Rigidbody::AddForce(const Vector3& a_vForce)
	{
		m_vForceAccumulated += a_vForce;
		m_bIsAwake = true;
	}

	void Rigidbody::ClearAccumulators()
	{
		m_vForceAccumulated.Clear();
		m_vTorqeAccumulated.Clear();
		m_bIsAwake = false;
	}

	void Rigidbody::AddForceAtBodyPoint(const Vector3& a_vForce, const Vector3& a_vPoint)
	{
		Vector3 _pt = GetPointInWorldSpace(a_vPoint);
		AddForceAtPoint(a_vForce, _pt);

		m_bIsAwake = true;
	}

	void Rigidbody::AddForceAtPoint(const Vector3& a_vForce, const Vector3& a_vPoint)
	{
		Vector3 _pt = a_vPoint;
		_pt -= m_vPosition;

		m_vForceAccumulated += a_vForce;
		m_vTorqeAccumulated += _pt.CrossProduct(a_vForce);

		m_bIsAwake = true;
	}

	Vector3 Rigidbody::GetPosition() const
	{
		return m_vPosition;
	}

	void Rigidbody::SetPosition(const Vector3& a_pos)
	{
		m_vPosition = a_pos;
	}

	Vector3 Rigidbody::GetVelocity() const
	{
		return m_vVelocity;
	}

	void Rigidbody::SetVelocity(const Vector3& a_vel)
	{
		m_vVelocity = a_vel;
	}

	number Rigidbody::GetDamping() const
	{
		return m_nLinearDamping;
	}

	void Rigidbody::SetDamping(const number a_damping)
	{
		m_nLinearDamping = a_damping;
	}

	number Rigidbody::GetMass() const
	{
		if (m_nInverseMass == 0)
		{
			return num_max;
		}
		else
		{
			return (number(1.0) / m_nInverseMass);
		}
	}

	number Rigidbody::GetInverseMass() const
	{
		return m_nInverseMass;
	}

	void Rigidbody::SetMass(const number a_mass)
	{
		assert(a_mass > 0);
		m_nInverseMass = number(1.0) / a_mass;
	}

	void Rigidbody::SetInverseMass(const number a_inverseMass)
	{
		m_nInverseMass = a_inverseMass;
	}

	bool Rigidbody::HasFiniteMass() const
	{
		return m_nInverseMass >= 0;
	}

	bool Rigidbody::GetBodyAwake() const
	{
		return m_bIsAwake;
	}

	void Rigidbody::SetAwake(const bool a_bAwake)
	{
		m_bIsAwake = a_bAwake;
	}

	bool Rigidbody::GetCanSleep() const
	{
		return m_bCanSleep;
	}

	void Rigidbody::SetCanSleep(const bool a_bCanSleep)
	{
		m_bCanSleep = a_bCanSleep;
	}

	number Rigidbody::GetAngularDamping() const
	{
		return m_nAngularDamping;
	}

	void Rigidbody::SetAngularDamping(const number a_nAngularDamping)
	{
		m_nAngularDamping = a_nAngularDamping;
	}

	Vector3 Rigidbody::GetPointInLocalSpace(const Vector3& point) const
	{
		return m_mTransformMatrix.TransformInverse(point);
	}

	Vector3 Rigidbody::GetPointInWorldSpace(const Vector3& point) const
	{
		return m_mTransformMatrix.Transform(point);
	}

	Vector3 Rigidbody::GetDirectionInLocalSpace(const Vector3& direction) const
	{
		return m_mTransformMatrix.TransformInverseDirection(direction);
	}

	Vector3 Rigidbody::GetDirectionInWorldSpace(const Vector3& direction) const
	{
		return m_mTransformMatrix.TransformDirection(direction);
	}

	void Rigidbody::GetTransformMatrix(Matrix3x4* a_mTransform) const
	{
		memcpy(a_mTransform, &m_mTransformMatrix.m_data, sizeof(Matrix3x4));
	}

	void Rigidbody::GetTransformMatrix(number a_mTransform[16]) const
	{
		memcpy(a_mTransform, m_mTransformMatrix.m_data, sizeof(number)*12);
		a_mTransform[12] = a_mTransform[13] = a_mTransform[14] = 0;
		a_mTransform[15] = 1;
	}

	void Rigidbody::GetGLTransform(float a_Transform[16]) const
	{
		a_Transform[0] = (float)m_mTransformMatrix.m_data[0];
		a_Transform[1] = (float)m_mTransformMatrix.m_data[4];
		a_Transform[2] = (float)m_mTransformMatrix.m_data[8];
		a_Transform[3] = 0;

		a_Transform[4] = (float)m_mTransformMatrix.m_data[1];
		a_Transform[5] = (float)m_mTransformMatrix.m_data[5];
		a_Transform[6] = (float)m_mTransformMatrix.m_data[9];
		a_Transform[7] = 0;

		a_Transform[8] = (float)m_mTransformMatrix.m_data[2];
		a_Transform[9] = (float)m_mTransformMatrix.m_data[6];
		a_Transform[10] = (float)m_mTransformMatrix.m_data[10];
		a_Transform[11] = 0;

		a_Transform[12] = (float)m_mTransformMatrix.m_data[3];
		a_Transform[13] = (float)m_mTransformMatrix.m_data[7];
		a_Transform[14] = (float)m_mTransformMatrix.m_data[11];
		a_Transform[15] = 1;
	}

	Matrix3x4 Rigidbody::GetTransformMatrix() const
	{
		return m_mTransformMatrix;
	}

	Matrix3x3 Rigidbody::GetInertiaTensor() const
	{
		Matrix3x3 _return;
		GetInertiaTensor(&_return);
		return _return;
	}

	void Rigidbody::GetInertiaTensor(Matrix3x3* a_outInertiaTensor) const
	{
		a_outInertiaTensor->SetInverse(m_mInverseInertiaTensor);
	}

	void Rigidbody::SetInertiaTensor(const Matrix3x3& a_inertiaTensor)
	{
		m_mInverseInertiaTensor.SetInverse(a_inertiaTensor);
	}

	Matrix3x3 Rigidbody::GetInertiaTensorWorld() const
	{
		Matrix3x3 _return;
		GetInertiaTensorWorld(&_return);
		return _return;
	}

	void Rigidbody::GetInertiaTensorWorld(Matrix3x3* a_outInertiaTensorWorld) const
	{
		a_outInertiaTensorWorld->SetInverse(m_mInverseInertiaTensorWorld);
	}

	void Rigidbody::SetInertiaTensorWorld(const Matrix3x3& a_inertiaTensorWorld)
	{
		m_mInverseInertiaTensorWorld.SetInverse(a_inertiaTensorWorld);
	}

	Quaternion Rigidbody::GetOrientation() const
	{
		return m_qOrientation;
	}

	void Rigidbody::GetOrientation(Quaternion& a_outOrientation) const
	{
		a_outOrientation = m_qOrientation;
	}

	void Rigidbody::SetOrientation(const Quaternion& a_qOrientation)
	{
		m_qOrientation = a_qOrientation;
	}

	Vector3 Rigidbody::GetRotation() const
	{
		return m_vRotation;
	}

	void Rigidbody::GetRotation(Vector3& a_outRotation) const
	{
		a_outRotation = m_vRotation;
	}

	void Rigidbody::SetRotation(const Vector3& a_vRotation)
	{
		m_vRotation = a_vRotation;
	}

	void Rigidbody::AddTorque(const Vector3& a_vDeltaTorque)
	{
		m_vTorqeAccumulated += a_vDeltaTorque;
	}

	void Rigidbody::AddRotation(const Vector3& a_vDeltaRotation)
	{
		m_vRotation += a_vDeltaRotation;
	}

	void Rigidbody::AddVelocity(const Vector3& a_vDeltaVelocity)
	{
		m_vVelocity += a_vDeltaVelocity;
	}

	Vector3 Rigidbody::GetAcceleration() const
	{
		return m_vAcceleration;
	}

	void Rigidbody::SetAcceleration(const Vector3& a_vAcceleration)
	{
		m_vAcceleration = a_vAcceleration;
	}

	void Rigidbody::Integrate(number a_nDeltaTime)
	{
		// calculate linear acceleration from force inputs.
		m_vLastFrameAcceleration = m_vAcceleration;
		m_vLastFrameAcceleration.AddScaledVector(m_vForceAccumulated, m_nInverseMass);

		// calculate angular acceleration from torque inputs.
		Vector3 _angularAcc = m_mInverseInertiaTensorWorld.Transform(m_vTorqeAccumulated);

		// Adjust velocities.
		// update linear from both acceleration and impulse.
		m_vVelocity.AddScaledVector(m_vLastFrameAcceleration, a_nDeltaTime);

		// update angular velocity.
		m_vRotation.AddScaledVector(m_vAngularAcceleration, a_nDeltaTime);

		// impose drag.
		m_vVelocity *= num_pow(m_nLinearDamping, a_nDeltaTime);
		m_vRotation *= num_pow(m_nAngularDamping, a_nDeltaTime);

		
		// Adjust position.
		// update linear position.
		m_vPosition.AddScaledVector(m_vVelocity, a_nDeltaTime);

		// update angular position.
		m_qOrientation.AddScaledVector(m_vRotation, a_nDeltaTime);

		// Normalize the orientation, and update the matrices 
		// with the new position and orientation.
		CalculateDerivedData();

		// Clear Accumulators.
		ClearAccumulators();
	}
}