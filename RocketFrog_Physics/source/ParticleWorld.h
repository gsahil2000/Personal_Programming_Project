#pragma once
#include <vector>
#include "ParticleContact.h"
#include "ParticleForceRegistry.h"
#include "ParticleContactResolver.h"

namespace RocketFrog
{
	/// @class ParticleWorld
	/// Keeps track of set of particles, and provides the means to update them all.
	class ParticleWorld
	{
	public:
		typedef std::vector<Particle*>					ParticleList;
		typedef std::vector<ParticleContactGenerator*>	ContactGeneratorList;

		/// creates new ParticleSimulator and sets the number of contacts and iterations to resolve them.
		/// if iterations is not passed, then its set to twice as many Contacts.
		ParticleWorld(const unsigned int a_nMaxContacts, const unsigned int a_nIterations=0);

		/// initializes the world for simulation frame. clears all previous forces.
		void StartFrame();


		/// holds the forceGenerators for the particles in this world.
		ParticleForceRegistry m_registry;

		/// holds the resolver for contacts.
		ParticleContactResolution m_resolver;

		/// contactGenerators.
		ContactGeneratorList m_contactGenList;

		/// list of Contacts.
		ParticleContact* m_pContacts;

		/// maxNumber of contacts allowed(i.e. size of contact list)
		unsigned int m_nMaxContacts;

		/// calls all contactGenerators to register their contacts and returns total number of contacts.
		unsigned int GenerateContacts();

		/// Integrates all particles in this world.
		void Integrate(const number a_nDeltaTime);

		/// processes all the physics for the particle world.
		void RunPhysics(const number a_nDeltaTime);

	protected:
		/// holds the particles.
		ParticleList m_particleList;

	private:
		bool m_bCalculateIterations;
	};
}