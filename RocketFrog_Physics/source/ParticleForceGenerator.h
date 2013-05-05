#pragma once
#include "Particle.h"
#include "Precision.h"

namespace RocketFrog
{
#pragma region ParticleForceGenerator
	/// @class ParticleForceGenerator
	/// A force generator can be asked to add force to one or more particles.
	class ParticleForceGenerator
	{
	public:
		ParticleForceGenerator(void);
		~ParticleForceGenerator(void);

		/// pure virtual function for the derived classes to implement.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime) = 0;
	};
#pragma endregion

#pragma region ParticleGravity
	/// @class ParticleGravity
	/// applies gravity to particles.
	class ParticleGravity : public ParticleForceGenerator
	{
	public:
		ParticleGravity(const Vector3& a_vGravity);

		/// function to apply gravity to the particle.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime);

		void   SetGravityModifier(const number a_nGravityModifier);
		number GetGravityModifier() const;

	private:
		/// holds the force that gravity applies.
		Vector3 m_vGravity;

		/// scalar number to modify gravity for a particle, defaults to 1.
		number  m_nGravityModifier;
	};
#pragma endregion

#pragma region ParticleDrag
	/// @class ParticleDrag
	/// Applies dragForce to particles.
	class ParticleDrag : public ParticleForceGenerator
	{
	public:
		/// ctor to set both the coefficients.
		ParticleDrag(number a_nVelCoefficient, number a_nVelSqCoefficient);

		/// apply drag force to particle.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime);

	private:
		/// velocity drag coefficient.
		number m_k1;

		/// velocity squared drag coefficient.
		number m_k2;
	};
#pragma endregion

#pragma region ParticleSpring
	/// @class ParticleSpring
	/// Generates Spring like force.
	class ParticleSpring : public ParticleForceGenerator
	{
	public:
		/// creates new spring with given params.
		ParticleSpring(Particle* a_pOther, const number a_nSpringConstant, const number a_nRestLength);
		
		/// applies spring force to other particle.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime);
	private:
		/// particle attached to the other end of spring.
		Particle* m_pOtherParticle;

		/// holds spring constant.
		number m_nSpringConstant;

		/// spring length at rest.
		number m_nRestLenght;
	};
#pragma endregion

#pragma region ParticleAnchoredSpring
	/// @class ParticleAnchoredSpring
	/// Another spring force generator with one end fixed at one place.
	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
	public:
		/// contructor to contruct a new Anchored spring with given params.
		ParticleAnchoredSpring(Vector3* a_pAnchorPosition, const number a_nSpringConstant, const number a_nRestLength);

		/// updates and applies the force to given particle.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime);

	protected:
		/// holds the position of the anchor(fixed) end of the spring.
		Vector3* m_pAnchorPosition;

		/// holds the spring constant;
		number m_nSpringConstant;

		/// holds the length of spring at rest.
		number m_nRestLength;
	};
#pragma endregion

#pragma region ParticleBungee
	/// @class ParticleBungee
	/// Force Generator to apply force that acts like Bungee Rope. Only applies force when extened.
	class ParticleBungee : public ParticleForceGenerator
	{
	public:
		/// constructs new bungee forceGenerator with given params.
		ParticleBungee(Particle* a_pOtherParticle, const number a_nSpringConstant, const number a_nRestLength);

		/// calculates and applies force to the particle.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime);

	private:
		/// particle at other end of the spring.
		Particle* m_pOtherParticle;

		/// holds the spring constant.
		number m_nSpringConstant;

		/// holds length of bungee beyond which it will start applying force.
		number m_nRestLength;
	};
#pragma endregion

#pragma region ParticleBuoyancy
	/// buouyancy force generator to keep the object afloat.
	/// assumes Y-Axis is up.
	class ParticleBuoyancy : public ParticleForceGenerator
	{
	public:
		/// created new buoyancy force with given params to calculate force.
		ParticleBuoyancy(const number a_maxDepth, const number a_volume, 
			const number a_liquidHeight, const number a_liquidDensity = 1000.0f);

		/// calculate and apply force.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime);

	private:
		/// max submersion depth of the object before it generates max buoyancy force.
		number m_nMaxDepth;

		/// volume of the object.
		number m_nVolume;

		/// holds water Level(height).
		number m_nLiquidLevel;

		/// density of the liquid. (for pure water its 1000kg/m^3)
		number m_nLiquidDensity;
	};
#pragma endregion

#pragma region ParticleFakeSpring
	/// @class ParticleFakeSpring
	/// Fake stiff spring forceGenerator, where one end is attached to fixed point in space.
	class ParticleFakeSpring : public ParticleForceGenerator
	{
	public:
		/// constructs new fakeStiffSpring with given params.
		ParticleFakeSpring(Vector3* a_pAnchorPosition, const number a_nSpringConstant, const number a_nDamping);

		/// calculates and applies force.
		virtual void UpdateForce(Particle* a_pParticle, const number a_nDeltaTime);

	private:
		/// The location of the anchored end of the spring.
		Vector3* m_pAnchorPosition;

		/// holds spring constant
		number m_nSpringConstatnt;

		/// holds the damping on the oscillation of the spring.
		number m_nDamping;
	};
#pragma endregion
}

