#pragma once
#include <Particle.h>

namespace RocketFrog
{
	class Fireworks : public Particle
	{
	public:

	private:
		unsigned int m_type;	///< type of firework particle.
		number		 m_age;		///< total time since this particle has spawned.

		struct FireParticleRule
		{
			number		m_minAge;	///< min time for which particle will be on screen.
			number		m_maxAge;	///< max time for which particle will be on screen.

			number		m_minVelocity;	///< min velocity of this particle.
			number		m_maxVelocity;	///< max velocity that the particle can reach.
			number		m_damping;		///< velocity drop over time.
		};
	};
}