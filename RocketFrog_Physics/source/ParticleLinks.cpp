#include "ParticleLinks.h"
#include <assert.h>

// Disable warning message 4100.
#pragma warning( push )
#pragma warning( disable : 4100 )

namespace RocketFrog
{
#pragma region ParticleLink
	number ParticleLink::GetCurrentLength() const
	{
		Vector3 _relativePos = m_pParticles[0]->GetPosition() - m_pParticles[1]->GetPosition();
		return _relativePos.Magnitude();
	}
#pragma endregion //ParticleLink

#pragma region ParticleCable
	unsigned int ParticleCable::AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const
	{
		assert(a_nLimit >= 1);

		/// Find length of the cable.
		number _length = GetCurrentLength();

		/// check if it is extended.
		if (_length < m_nMaxLength)
		{
			return 0;
		}

		/// FILL THE CONTACT CLASS MEMBERS.
		/// Return the contact.
		a_pContact->SetParticle(m_pParticles[0], 0);
		a_pContact->SetParticle(m_pParticles[1], 1);

		/// calculate the normal.
		Vector3 _normal = m_pParticles[1]->GetPosition() - m_pParticles[0]->GetPosition();
		_normal.Normalize();
		a_pContact->SetContactNormal(_normal);

		a_pContact->SetContactPenetration(_length - m_nMaxLength);
		a_pContact->SetRestitutionCoefficient(m_nRestitution);

		return 1;
	}
#pragma endregion //ParticleCable

#pragma region ParticleRod
	unsigned int ParticleRod::AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const
	{
		assert(a_nLimit >= 1);

		/// find the length of the rod.
		number _currentLength = GetCurrentLength();

		/// check if we're overextended.
		if (_currentLength == m_nLength)
		{
			return 0;
		}

		/// FILL THE CONTACT MEMBERS.
		a_pContact->SetParticle(m_pParticles[0], 0);
		a_pContact->SetParticle(m_pParticles[1], 1);

		/// calculate the normal.
		Vector3 _normal = m_pParticles[1]->GetPosition() - m_pParticles[0]->GetPosition();
		_normal.Normalize();

		/// Check if we are extending or compressing.
		if (_currentLength > m_nLength)
		{
			a_pContact->SetContactNormal(_normal);
			a_pContact->SetContactPenetration(_currentLength - m_nLength);
		}
		else
		{
			a_pContact->SetContactNormal(_normal*-1);
			a_pContact->SetContactPenetration(m_nLength - _currentLength);
		}

		/// alawys use zero restitution to avoid any bounciness.
		a_pContact->SetRestitutionCoefficient(0);

		return 1;
	}
#pragma endregion //ParticleRod

#pragma region ParticleContraint
	number ParticleConstraint::CurrentLength() const
	{
		Vector3 _relativePos = m_pParticle->GetPosition() - m_vAnchorPosition;
		return _relativePos.Magnitude();
	}

	Particle* ParticleConstraint::GetParticle()
	{
		return m_pParticle;
	}

	void ParticleConstraint::SetParticle(Particle* a_pParticle)
	{
		m_pParticle = a_pParticle;
	}

	Vector3 ParticleConstraint::GetAnchorPosition() const
	{
		return m_vAnchorPosition;
	}

	void ParticleConstraint::SetAnchorPosition(Vector3& a_vAnchorPosition)
	{
		m_vAnchorPosition = a_vAnchorPosition;
	}
#pragma endregion //ParticleConstraint

#pragma region ParticleCableConstraint
	number ParticleCableConstaint::GetMaxLength() const
	{
		return m_nMaxLength;
	}

	void ParticleCableConstaint::SetMaxLenth(number a_nMaxLength)
	{
		m_nMaxLength = a_nMaxLength;
	}

	number ParticleCableConstaint::GetRestitution() const
	{
		return m_nRestitution;
	}

	void ParticleCableConstaint::SetRestitution(number a_nRestitution)
	{
		m_nRestitution = a_nRestitution;
	}

	unsigned int ParticleCableConstaint::AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const
	{
		// get current legth.
		number _length = CurrentLength();

		// check if it is stretching.
		if (_length < m_nMaxLength)
		{
			return 0;
		}

		// otherwise return this contact to be resolved.
		a_pContact->SetParticle(m_pParticle, 0);
		a_pContact->SetParticle(nullptr, 1);

		// calculate the normal.
		Vector3 _normal = m_vAnchorPosition - m_pParticle->GetPosition();
		_normal.Normalize();
		a_pContact->SetContactNormal(_normal);

		a_pContact->SetContactPenetration(_length - m_nMaxLength);
		a_pContact->SetRestitutionCoefficient(m_nRestitution);

		return 1;
	}
#pragma endregion //ParticleCableConstraint

#pragma region ParticleRodConstraint
	number ParticleRodConstraint::GetLength() const
	{
		return m_nLength;
	}

	void ParticleRodConstraint::SetLength(number a_nLength)
	{
		m_nLength = a_nLength;
	}

	unsigned int ParticleRodConstraint::AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const
	{
		// find the current length;
		number _length = CurrentLength();

		// check it action is required.
		if (_length == m_nLength)
		{
			return 0;
		}

		a_pContact->SetParticle(m_pParticle, 0);
		a_pContact->SetParticle(nullptr, 1);

		Vector3 _normal = m_vAnchorPosition - m_pParticle->GetPosition();
		_normal.Normalize();

		if (_length > m_nLength)
		{
			a_pContact->SetContactNormal(_normal);
			a_pContact->SetContactPenetration(_length - m_nLength);
		}
		else
		{
			a_pContact->SetContactNormal(_normal * -1);
			a_pContact->SetContactPenetration(m_nLength - _length);
		}

		a_pContact->SetRestitutionCoefficient(0);

		return 1;
	}
#pragma endregion //ParticleRodConstraint
}

#pragma warning (pop)