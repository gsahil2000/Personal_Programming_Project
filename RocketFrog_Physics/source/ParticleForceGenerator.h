#pragma once
#include "Particle.h"
#include "Precision.h"

namespace RocketFrog
{
	/// @class ParticleForceGenerator
	/// A force generator can be asked to add force to one or more particles.
	class ParticleForceGenerator
	{
	public:
		ParticleForceGenerator(void);
		~ParticleForceGenerator(void);

		/// pure virtual function for the derived classes to implement.
		virtual void UpdateForce(Particle* a_pParticle, number a_nDeltaTime) = 0;
	};

	/// @class ParticleGravity
	/// applies gravity to particles.
	class ParticleGravity : public ParticleForceGenerator
	{
	public:
		ParticleGravity(const Vector3& a_vGravity);

		/// function to apply gravity to the particle.
		virtual void UpdateForce(Particle* a_pParticle, number a_nDeltaTime);

		void   SetGravityModifier(const number a_nGravityModifier);
		number GetGravityModifier() const;

	private:
		/// holds the force that gravity applies.
		Vector3 m_vGravity;

		/// scalar number to modify gravity for a particle, defaults to 1.
		number  m_nGravityModifier;
	};


	/// @class ParticleDrag
	/// Applies dragForce to particles.
	class ParticleDrag : public ParticleForceGenerator
	{
	public:
		/// ctor to set both the coefficients.
		ParticleDrag(number a_nVelCoefficient, number a_nVelSqCoefficient);

		/// apply drag force to particle.
		virtual void UpdateForce(Particle* a_pParticle, number a_nDeltaTime);

	private:
		/// velocity drag coefficient.
		number m_k1;

		/// velocity squared drag coefficient.
		number m_k2;
	};
}

