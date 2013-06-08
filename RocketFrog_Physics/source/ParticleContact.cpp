#include "ParticleContact.h"
#include <assert.h>

namespace RocketFrog
{
	Particle* ParticleContact::GetParticle(const unsigned short a_nIndex) const
	{
		assert(a_nIndex < 2);
		return m_pParticles[a_nIndex];
	}

	void ParticleContact::SetParticle(Particle* const a_pParticle, const unsigned short a_nIndex)
	{
		assert(a_nIndex < 2);
		m_pParticles[a_nIndex] = a_pParticle;
	}

	number ParticleContact::GetRestitutionCoefficient() const
	{
		return m_nRestitutionCoeff;
	}

	void ParticleContact::SetRestitutionCoefficient(const number a_nValue)
	{
		m_nRestitutionCoeff = a_nValue;
	}

	Vector3 ParticleContact::GetContactNormal() const
	{
		return m_vContactNormal;
	}

	void ParticleContact::SetContactNormal(const Vector3& a_vContactNormal)
	{
		m_vContactNormal = a_vContactNormal;
	}

	number ParticleContact::GetContactPenetration() const
	{
		return m_nPenetration;
	}

	void ParticleContact::SetContactPenetration(const number a_nPenetration)
	{
		m_nPenetration = a_nPenetration;
	}

	void ParticleContact::Resolve(const number a_nDeltaTime)
	{
		ResolveVelocity(a_nDeltaTime);
	}

	number ParticleContact::CalculateSeparatingVelocity() const
	{
		Vector3 _relativeVel = m_pParticles[0]->GetVelocity();
		if (m_pParticles[1] != nullptr)
		{
			_relativeVel -= m_pParticles[1]->GetVelocity();
		}
		return _relativeVel.DotProduct(m_vContactNormal);
	}

	//void ParticleContact::ResolveVelocity(const number a_nDeltaTime)
	//{
	//	/// find the velocity in direction of contact.
	//	number _separatingVelocity = CalculateSeparatingVelocity();

	//	/// check if it needs to be resolved.
	//	if (_separatingVelocity > 0)
	//	{
	//		/// the contact is either separating or stationary. No action required.
	//		return;
	//	}

	//	/// calculate new separating velocity.
	//	number _newSepVel = -_separatingVelocity * m_nRestitutionCoeff;
	//	number _deltaVel  = _newSepVel - _separatingVelocity;

	//	/// apply change in velocity to each object in proportion to inverse of their masses.
	//	number _totalInverseMass = m_pParticles[0]->GetInverseMass();
	//	if (m_pParticles[1] != nullptr)
	//	{
	//		_totalInverseMass += m_pParticles[1]->GetInverseMass();
	//	}

	//	/// check total inverseMass, if both have infinite then no action required.
	//	if (_totalInverseMass <= 0)
	//		return;

	//	/// calculate the impulse to apply.
	//	number _impulse = _deltaVel / _totalInverseMass;

	//	/// calculate impulse per unit of inverse mass.
	//	Vector3 _impulsePerInvMass = m_vContactNormal * _impulse;
	//	
	//	/// Apply impulses in direction of contactNormal and proportional to InverseMasses.
	//	m_pParticles[0]->SetVelocity(m_pParticles[0]->GetVelocity() +
	//						_impulsePerInvMass * m_pParticles[0]->GetInverseMass());

	//	if (m_pParticles[1] != nullptr)
	//	{
	//		m_pParticles[1]->SetVelocity(m_pParticles[1]->GetVelocity() +
	//			_impulsePerInvMass * m_pParticles[1]->GetInverseMass());
	//	}
	//}

	void ParticleContact::ResolveVelocity(const number a_nDeltaTime)
	{
		/// find the velocity in direction of contact.
		number _separatingVelocity = CalculateSeparatingVelocity();

		/// check if it needs to be resolved.
		if (_separatingVelocity > 0)
		{
			/// the contact is either separating or stationary. No action required.
			return;
		}

		/// calculate new separating velocity.
		number _newSepVel = -_separatingVelocity * m_nRestitutionCoeff;

		/// *** Extra work for resting objects ***************
		/// check the velocity buildup due to acceleration only.
		Vector3 _accCausedVel = m_pParticles[0]->GetAcceleration();
		if (m_pParticles[1])
		{
			_accCausedVel -= m_pParticles[1]->GetAcceleration();
		}

		number _accCausedSepVel = _accCausedVel.DotProduct(m_vContactNormal) * a_nDeltaTime;

		/// if we've closing velocity due to acceleration buildup,
		/// remove it from the new separating velocity.
		if (_accCausedSepVel < 0)
		{
			_newSepVel += m_nRestitutionCoeff * _accCausedSepVel;

			/// make sure we've not removed more than required.
			if (_newSepVel < 0)
			{
				_newSepVel = 0;
			}
		}
		/// *** Resting object part end. **********************

		number _deltaVel  = _newSepVel - _separatingVelocity;

		/// apply change in velocity to each object in proportion to inverse of their masses.
		number _totalInverseMass = m_pParticles[0]->GetInverseMass();
		if (m_pParticles[1] != nullptr)
		{
			_totalInverseMass += m_pParticles[1]->GetInverseMass();
		}

		/// check total inverseMass, if both have infinite then no action required.
		if (_totalInverseMass <= 0)
			return;

		/// calculate the impulse to apply.
		number _impulse = _deltaVel / _totalInverseMass;

		/// calculate impulse per unit of inverse mass.
		Vector3 _impulsePerInvMass = m_vContactNormal * _impulse;
		
		/// Apply impulses in direction of contactNormal and proportional to InverseMasses.
		m_pParticles[0]->SetVelocity(m_pParticles[0]->GetVelocity() +
							_impulsePerInvMass * m_pParticles[0]->GetInverseMass());

		if (m_pParticles[1] != nullptr)
		{
			m_pParticles[1]->SetVelocity(m_pParticles[1]->GetVelocity() +
				_impulsePerInvMass * -m_pParticles[1]->GetInverseMass());
		}
	}

	void ParticleContact::ResolveInterPenetration(/*const number a_nDeltaTime*/)
	{
		/// check if there is a need to take action.
		if (m_nPenetration <= 0)
			return;

		/// total InvMass of both objects coz their final movement will be based on that.
		number _totalInvMass = m_pParticles[0]->GetInverseMass();
		if (m_pParticles[1])
		{
			_totalInvMass += m_pParticles[1]->GetInverseMass();
		}

		/// return if both the particles have infinite mass.
		if (_totalInvMass <= 0)
			return;

		/// find the contact resolution per object.
		Vector3 _movePerInvMass = m_vContactNormal * (m_nPenetration / _totalInvMass);

		/// calculate the movement amounts and penetration resolution.
		m_vParticleMovement[0] = _movePerInvMass * m_pParticles[0]->GetInverseMass();
		m_pParticles[0]->SetPosition(m_pParticles[0]->GetPosition() + m_vParticleMovement[0]);

		if (m_pParticles[1])
		{
			m_vParticleMovement[1] = _movePerInvMass * -m_pParticles[1]->GetInverseMass();
			m_pParticles[1]->SetPosition(m_pParticles[1]->GetPosition() + m_vParticleMovement[1]);
		}
		else
		{
			m_vParticleMovement[1].Clear();
		}
	}
}