#pragma once
#include "ParticleContact.h"

namespace RocketFrog
{
	/// @class ParticleContactResolver
	/// The contact resolution routine for particles.
	/// Once instance of this class can be used for entire simulation.
	class ParticleContactResolution
	{
	public:
		/// Default Ctor.
		ParticleContactResolution();

		/// contructs new contactResolution and sets the number of iterations allowed.
		ParticleContactResolution(const unsigned int a_nIterations);

		/// no. of iterations accessor.
		void		 SetIterations(const unsigned int a_nIterations);
		unsigned int GetIterations() const;

		/// resolves the set of particle contacts for both penetration and velocity.
		void ResolveContacts(ParticleContact* a_pContacts, unsigned int a_nContactCount, number a_nDeltaTime);

	protected:
		/// holds the number of iterations allowed for this contact resolution.
		unsigned int m_nIterations;

		/// iterations already used.
		unsigned int m_nIterationsUsed;

	private:

	};
}