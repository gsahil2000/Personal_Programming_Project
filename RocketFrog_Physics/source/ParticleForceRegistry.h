#pragma once
#include "ParticleForceGenerator.h"
#include <vector>

namespace RocketFrog
{
	/// @class ParticleForceRegistry
	/// Holds all the forceGenerators and Particles that they apply force to.
	class ParticleForceRegistry
	{
	protected:
		/// @struct ParticleForceRegistration
		/// Holds the forceGenerator and Particle to apply force to.
		struct ParticleForceRegistration
		{
			Particle*				m_pParticle;
			ParticleForceGenerator* m_pPFG;

			ParticleForceRegistration(Particle* a_pParticle, ParticleForceGenerator* a_pPFG);
		};

		/// list of all ParticleForceRegistrations
		typedef std::vector<ParticleForceRegistration> Registry;
		Registry m_registration;

	public:
		ParticleForceRegistry(void);
		~ParticleForceRegistry(void);

		/// registers the given forceGenerator to apply force to given particle.
		void Add(Particle* a_pParticle, ParticleForceGenerator* a_pPfg);

		/// removes the given pair from the vector(if it exists).
		void Remove(Particle* a_pParticle, ParticleForceGenerator* a_pPfg);

		/// clears the registration vector.
		void Clear();

		/// calls all forceGens to apply forces to their particles.
		void UpdateForces(number a_nDeltaTime);
	};
}