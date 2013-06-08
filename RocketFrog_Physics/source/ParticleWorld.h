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

		/// delete the simulator
		~ParticleWorld();

		/// initializes the world for simulation frame. clears all previous forces.
		void StartFrame();

		/// calls all contactGenerators to register their contacts and returns total number of contacts.
		unsigned int GenerateContacts();

		/// Integrates all particles in this world.
		void Integrate(const number a_nDeltaTime);

		/// processes all the physics for the particle world.
		void RunPhysics(const number a_nDeltaTime);

		/// ParticleList Getter.
		ParticleList* GetParticleList();

		/// Contact Generator Getter.
		ContactGeneratorList* GetContactGeneratorList();

		// Particle Force Registry Getter.
		ParticleForceRegistry* GetParticleForceRegistry();

	protected:
		/// holds the particles.
		ParticleList m_particleList;

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

		bool m_bCalculateIterations;

	private:
	};

	class ParticleGroundContactGenerator : public ParticleContactGenerator
	{
		ParticleWorld::ParticleList* m_particleList;

	public:
		void Init(ParticleWorld::ParticleList* a_particleList);

		virtual unsigned int AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const;
	};
}