#include "ParticleLinks.h"
#include <assert.h>

namespace RocketFrog
{
	number ParticleLink::GetCurrentLength() const
	{
		Vector3 _relativePos = m_pParticles[0]->GetPosition() - m_pParticles[1]->GetPosition();
		return _relativePos.Magnitude();
	}

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

	unsigned int ParticleRod::AddContact(ParticleContact* a_pContact, unsigned int a_nLimit) const
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
}