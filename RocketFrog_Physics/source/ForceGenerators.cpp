#include "ForceGenerators.h"

#pragma warning( push )
#pragma warning( disable : 4100 )

namespace RocketFrog
{
#pragma region Gravity
	void GravityForce::UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime)
	{
		if (!a_pBody->HasFiniteMass())
			return;

		// apply the mass scaled force.
		a_pBody->AddForce(m_vGravity * m_nGravityModifier * a_pBody->GetMass());
	}
#pragma endregion //Gravity

#pragma region Spring
	Spring::Spring(const Vector3& a_vLocalConnectionPt,
			Rigidbody* a_pOtherBody,
			const Vector3& a_vOtherConnectionPt,
			number a_nSpringConstant,
			number a_nRestLength)
			:m_vConnectionPoint(a_vLocalConnectionPt), 
			m_pOtherBody(a_pOtherBody), m_vOtherConnectionPoint(a_vOtherConnectionPt),
			m_nSpringConstant(a_nSpringConstant), m_nRestLength(a_nRestLength)
	{}

	void Spring::UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime)
	{
		Vector3 _lws = a_pBody->GetPointInLocalSpace(m_vConnectionPoint);
		Vector3 _ows = m_pOtherBody->GetPointInWorldSpace(m_vOtherConnectionPoint);

		// calculate the spring vector.
		Vector3 _force = _lws - _ows;

		// calculate the magnitude of the force.
		number _magnitude = _force.Magnitude();
		_magnitude = abs(_magnitude - m_nRestLength);
		_magnitude *= m_nSpringConstant;

		// calculate the final force to apply.
		_force.Normalize();
		_force *= -_magnitude;
		a_pBody->AddForceAtPoint(_force, _lws);
	}
#pragma endregion //Spring

#pragma region Aero
	Aero::Aero(const Matrix3x3& a_mTensor, const Vector3& a_vPosition, const Vector3* a_pWindSpeed)
		:m_mTensor(a_mTensor), m_vPosition(a_vPosition), m_vWindSpeed(a_pWindSpeed)
	{}

	void Aero::UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime)
	{
		UpdateForceFromTensor(a_pBody, a_nDeltaTime, m_mTensor);
	}

	void Aero::UpdateForceFromTensor(Rigidbody* a_pBody, const number a_nDeltaTime, const Matrix3x3& a_mTensor)
	{
		// calculate total velocity (wind speed and body's velocity)
		Vector3 _velocity = a_pBody->GetVelocity();
		_velocity += *m_vWindSpeed;

		// calculate the velocity in local space.
		Vector3 _localVel = a_pBody->GetTransformMatrix().TransformInverseDirection(_velocity);

		// calcualte the force in local space.
		Vector3 _localForce = a_mTensor.Transform(_localVel);
		Vector3 _force = a_pBody->GetTransformMatrix().TransformDirection(_localForce);

		// apply the force.
		a_pBody->AddForceAtBodyPoint(_force, m_vPosition);
	}
#pragma endregion //Aero

#pragma region AeroControl
	AeroControl::AeroControl(const Matrix3x3& a_mBase, const Matrix3x3& a_mMin, const Matrix3x3& a_mMax,
				const Vector3& a_vPosition, const Vector3* a_pWindSpeed)
				:Aero(a_mBase, a_vPosition, a_pWindSpeed), 
				m_mMaxTensor(a_mMax), m_mMinTensor(a_mMin),
				m_nControlSettings(0.0f)
	{}

	void AeroControl::SetControl(const number a_nValue)
	{
		m_nControlSettings = a_nValue;
	}

	void AeroControl::UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime)
	{
		UpdateForceFromTensor(a_pBody, a_nDeltaTime, GetTensor());
	}

	Matrix3x3 AeroControl::GetTensor()
	{
		if (m_nControlSettings <= -1.0f)
		{
			return m_mMinTensor;
		}
		else if (m_nControlSettings >= 1.0f)
		{
			return m_mMaxTensor;
		}
		else if (m_nControlSettings < 0)
		{
			return Matrix3x3::LinearInterpolate(m_mMinTensor, m_mTensor, m_nControlSettings+1.0f);
		}
		else if (m_nControlSettings > 0)
		{
			return Matrix3x3::LinearInterpolate(m_mTensor, m_mMaxTensor, m_nControlSettings);
		}
		else 
			return m_mTensor;
	}

#pragma endregion //AeroControl

#pragma region ForceRegistry
	void ForceRegistry::Add(Rigidbody* a_pBody, ForceGenerator* a_pFG)
	{
		ForceRegistry::ForceRegistration registration;
		registration.m_pBody = a_pBody;
		registration.m_pFG = a_pFG;
		m_Registrations.push_back(registration);
	}

	void ForceRegistry::Remove(Rigidbody* a_pBody, ForceGenerator* a_pFG)
	{
		RegistryList::iterator _it = m_Registrations.begin();

		while (_it != m_Registrations.end())
		{
			if (_it->m_pBody == a_pBody && _it->m_pFG == a_pFG)
			{
				m_Registrations.erase(_it);
				break;
			}
			++_it;
		}
	}

	void ForceRegistry::Clear()
	{
		m_Registrations.clear();
	}

	void ForceRegistry::UpdateForces(const number a_nDeltaTime)
	{
		for (RegistryList::iterator _it = m_Registrations.begin(); _it != m_Registrations.end(); ++_it)
		{
			_it->m_pFG->UpdateForce(_it->m_pBody, a_nDeltaTime);
		}
	}

#pragma endregion //ForceRegistry
}

#pragma warning ( pop )