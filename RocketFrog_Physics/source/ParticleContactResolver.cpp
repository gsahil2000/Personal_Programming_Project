#include "ParticleContactResolver.h"

namespace RocketFrog
{
	ParticleContactResolution::ParticleContactResolution()
		:m_nIterations(10), m_nIterationsUsed(0)
	{}

	ParticleContactResolution::ParticleContactResolution(const unsigned int a_nIterations)
		:m_nIterations(a_nIterations), m_nIterationsUsed(0)
	{}

	void ParticleContactResolution::SetIterations(const unsigned int a_nIterations)
	{
		m_nIterations = a_nIterations;
	}

	unsigned int ParticleContactResolution::GetIterations() const
	{
		return m_nIterations;
	}

	void ParticleContactResolution::ResolveContacts(ParticleContact* a_pContacts, unsigned int a_nContactCount, number a_nDeltaTime)
	{
		m_nIterationsUsed = 0;
		while (m_nIterationsUsed < m_nIterations)
		{
			/// find the contact with larget closing velocity.
			number		 _max	   = num_max;
			unsigned int _maxIndex = a_nContactCount;
			for (unsigned int i = 0; i < a_nContactCount; ++i)
			{
				number _sepVel = a_pContacts[i].CalculateSeparatingVelocity();
				if (_sepVel < _max && (_sepVel < 0 || a_pContacts[i].m_nPenetration > 0))
				{
					_max	  = _sepVel;
					_maxIndex = i;
				}
			}

			/// check if there is anything to be resolved?
			if (_maxIndex == a_nContactCount)
				break;

			/// resolve this contact.
			a_pContacts[_maxIndex].Resolve(a_nDeltaTime);

			/// update iterationsUsed.
			++m_nIterationsUsed;
		}
	}
}