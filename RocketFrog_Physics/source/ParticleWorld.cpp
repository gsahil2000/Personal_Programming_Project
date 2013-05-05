#include "ParticleWorld.h"

namespace RocketFrog
{
	void ParticleWorld::StartFrame()
	{
		for (ParticleList::iterator _it = m_particleList.begin(); _it != m_particleList.end(); ++_it)
		{
			/// iterate through all particles and clear accumulated forces.
			(*_it)->ClearForces();
		}
	}

	unsigned int ParticleWorld::GenerateContacts()
	{
		unsigned int _limit = m_nMaxContacts;
		ParticleContact* _nextContact = m_pContacts;

		for (ContactGeneratorList::iterator _it = m_contactGenList.begin(); _it != m_contactGenList.end(); ++_it)
		{
			unsigned int _used = (*_it)->AddContact(_nextContact, _limit);
			_limit -= _used;
			_nextContact += _used;

			/// if we run out of contacts to fill. This means something contacts.
			if (_limit <= 0)
				break;
		}

		/// return the number of contacts used.
		return m_nMaxContacts - _limit;
	}

	void ParticleWorld::Integrate(const number a_nDeltaTime)
	{
		for (ParticleList::iterator _it = m_particleList.begin(); _it != m_particleList.end(); ++_it)
		{
			/// call integrate on all particles to update them.
			(*_it)->Integrate(a_nDeltaTime);
		}
	}

	void ParticleWorld::RunPhysics(const number a_nDeltaTime)
	{
		/// Update all the forces.
		m_registry.UpdateForces(a_nDeltaTime);

		/// integrate the objects.
		Integrate(a_nDeltaTime);

		/// generate contacts.
		unsigned int _usedContacts = GenerateContacts();

		/// process all the contacts.
		if (_usedContacts)
		{
			if (m_bCalculateIterations)
			{
				m_resolver.SetIterations(_usedContacts * 2);
				m_resolver.ResolveContacts(m_pContacts, _usedContacts, a_nDeltaTime);
			}
		}
	}
}