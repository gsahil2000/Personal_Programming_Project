#include "ParticleForceGenerator.h"

namespace RocketFrog
{
	ParticleForceGenerator::ParticleForceGenerator(void)
	{	}
	
	
	ParticleForceGenerator::~ParticleForceGenerator(void)
	{	}

	ParticleGravity::ParticleGravity(const Vector3& a_vGravity)
		:m_vGravity(a_vGravity), m_nGravityModifier(1.0)
	{	}

	void ParticleGravity::UpdateForce(Particle* a_pParticle, number a_nDeltaTime)
	{
		if (a_pParticle->HasFiniteMass())
			return;

		a_nDeltaTime++;
		a_pParticle->AddForce(m_vGravity * m_nGravityModifier * a_pParticle->GetMass());
	}

	void ParticleGravity::SetGravityModifier(number a_nGravityModifier)
	{
		if (a_nGravityModifier > 1.0)
		{
			m_nGravityModifier = 1.0;
		}
		else if (a_nGravityModifier < 0.0)
		{
			m_nGravityModifier = 0.0;
		}
		else
		{
			m_nGravityModifier = a_nGravityModifier;
		}
	}

	number ParticleGravity::GetGravityModifier() const
	{
		return m_nGravityModifier;
	}

	ParticleDrag::ParticleDrag(number a_nVelCoefficient, number a_nVelSqCoefficient)
		:m_k1(a_nVelCoefficient), m_k2(a_nVelSqCoefficient)
	{	}

	void ParticleDrag::UpdateForce(Particle* a_pParticle, number a_nDeltaTime)
	{
		Vector3 _force;
		_force = a_pParticle->GetVelocity();

		/// calculate drag force
		number _dragCoeff = _force.Magnitude();
		_dragCoeff = (m_k1 * _dragCoeff) + (m_k2 * _dragCoeff * _dragCoeff);

		_force.Normalize();
		_force *= -_dragCoeff;

		a_nDeltaTime++;
		/// apply the force
		a_pParticle->AddForce(_force);
	}
}