#include "ParticleWorld.h"

namespace RocketFrog
{
#pragma region ParticleWorld
	ParticleWorld::ParticleWorld(const unsigned int a_nMaxContacts, const unsigned int a_nIterations)
		:m_resolver(a_nIterations), m_nMaxContacts(a_nMaxContacts)
	{
		m_pContacts            = new ParticleContact[m_nMaxContacts];
		m_bCalculateIterations = (a_nIterations == 0);
	}

	ParticleWorld::~ParticleWorld()
	{
		delete[] m_pContacts;
	}

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
		unsigned int     _limit       = m_nMaxContacts;
		ParticleContact* _nextContact = m_pContacts;

		for (ContactGeneratorList::iterator _it = m_contactGenList.begin(); _it != m_contactGenList.end(); ++_it)
		{
			unsigned int _used = (*_it)->AddContact(_nextContact, _limit);
			_limit       -= _used;
			_nextContact += _used;

			/// if we run out of contacts to fill. This means something contacts.
			if (_limit <= 0)
			{
				break;
			}
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
			}
			m_resolver.ResolveContacts(m_pContacts, _usedContacts, a_nDeltaTime);
		}
	}

	ParticleWorld::ParticleList* ParticleWorld::GetParticleList()
	{
		return &m_particleList;
	}

	ParticleWorld::ContactGeneratorList* ParticleWorld::GetContactGeneratorList()
	{
		return &m_contactGenList;
	}

	ParticleForceRegistry* ParticleWorld::GetParticleForceRegistry()
	{
		return &m_registry;
	}
#pragma endregion //ParticleWorld	

#pragma region ParticleGroundContactGenerator
	void ParticleGroundContactGenerator::Init(ParticleWorld::ParticleList* a_particleList)
	{
		m_particleList = a_particleList;
	}

	unsigned int ParticleGroundContactGenerator::AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const
	{
		unsigned int _count = 0;
		ParticleWorld::ParticleList::iterator it = m_particleList->begin();

		for (; it != m_particleList->end(); ++it)
		{
			number _y = (*it)->GetPosition().y;
			if (_y < 0.0f)
			{
				a_pContact->SetContactNormal(Vector3::UP);
				a_pContact->SetParticle(*it, 0);
				a_pContact->SetParticle(nullptr, 1);
				a_pContact->SetContactPenetration(-_y);
				a_pContact->SetRestitutionCoefficient(0.2f);
				a_pContact++;
				_count++;
			}
			if (_count >= a_nLimit)
			{
				return _count;
			}
		}

		return _count;
	}
#pragma endregion //ParticleGroundContactGenerator
}