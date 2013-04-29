#include "ParticleForceRegistry.h"
#include <assert.h>


namespace RocketFrog
{
	ParticleForceRegistry::ParticleForceRegistry(void)
		:m_registration()
	{
	}
	
	
	ParticleForceRegistry::~ParticleForceRegistry(void)
	{
	}
	
	ParticleForceRegistry::ParticleForceRegistration::ParticleForceRegistration(Particle* a_pParticle, ParticleForceGenerator* a_pPFG)
		:m_pParticle(a_pParticle), m_pPFG(a_pPFG)
	{}
	
	void ParticleForceRegistry::Add(Particle* a_pParticle, ParticleForceGenerator* a_pPfg)
	{
		assert(a_pParticle != nullptr);
		assert(a_pPfg != nullptr);

		ParticleForceRegistration _temp(a_pParticle, a_pPfg);
		m_registration.push_back(_temp);
	}

	void ParticleForceRegistry::Remove(Particle* a_pParticle, ParticleForceGenerator* a_pPfg)
	{
		Registry::iterator _it = m_registration.begin();
		for (; _it != m_registration.end(); ++_it)
		{
			if (_it->m_pParticle == a_pParticle && _it->m_pPFG == a_pPfg)
			{
				m_registration.erase(_it);
				break;
			}
		}
	}

	void ParticleForceRegistry::Clear()
	{
		m_registration.clear();
	}

	void ParticleForceRegistry::UpdateForces(number a_nDeltaTime)
	{
		Registry::iterator _it = m_registration.begin();
		for (; _it != m_registration.end(); ++_it)
		{
			_it->m_pPFG->UpdateForce(_it->m_pParticle, a_nDeltaTime);
		}
	}
}