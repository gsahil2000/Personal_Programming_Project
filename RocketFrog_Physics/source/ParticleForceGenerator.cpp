#include "ParticleForceGenerator.h"

// Disable warning message 4100.
#pragma warning( push )
#pragma warning( disable : 4100 )

namespace RocketFrog
{
#pragma region ParticleForceGenerator
	ParticleForceGenerator::ParticleForceGenerator(void)
	{	}
	
	
	ParticleForceGenerator::~ParticleForceGenerator(void)
	{	}
#pragma endregion

#pragma region ParticleGravity
	ParticleGravity::ParticleGravity(const Vector3& a_vGravity)
		:m_vGravity(a_vGravity), m_nGravityModifier(1.0)
	{	}

	void ParticleGravity::UpdateForce(Particle* a_pParticle, const number a_nDeltaTime)
	{
		if (!a_pParticle->HasFiniteMass())
			return;

		//a_nDeltaTime++;
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
#pragma endregion

#pragma region ParticleGravity
	number ParticleGravity::GetGravityModifier() const
	{
		return m_nGravityModifier;
	}

	ParticleDrag::ParticleDrag(number a_nVelCoefficient, number a_nVelSqCoefficient)
		:m_k1(a_nVelCoefficient), m_k2(a_nVelSqCoefficient)
	{	}

	void ParticleDrag::UpdateForce(Particle* a_pParticle, const number a_nDeltaTime)
	{
		Vector3 _force;
		_force = a_pParticle->GetVelocity();

		/// calculate drag force
		number _dragCoeff = _force.Magnitude();
		_dragCoeff = (m_k1 * _dragCoeff) + (m_k2 * _dragCoeff * _dragCoeff);

		_force.Normalize();
		_force *= -_dragCoeff;

		/// apply the force
		a_pParticle->AddForce(_force);
	}
#pragma endregion

#pragma region ParticleSpring
	ParticleSpring::ParticleSpring(Particle* a_pOther, const number a_nSpringConstant, const number a_nRestLength)
		:m_pOtherParticle(a_pOther), m_nSpringConstant(a_nSpringConstant), m_nRestLenght(a_nRestLength)
	{	}

	void ParticleSpring::UpdateForce(Particle* a_pParticle, const number a_nDeltaTime)
	{
		/// Calculate the vector of the spring.
		Vector3 _force(a_pParticle->GetPosition());
		_force -= m_pOtherParticle->GetPosition();

		/// calculate the magnitude of the force.
		number _magnitude = _force.Magnitude();
		_magnitude = fabs(_magnitude - m_nRestLenght);
		_magnitude *= m_nSpringConstant;

		/// calculate the final force and apply it.
		_force.Normalize();
		_force *= -_magnitude;
		a_pParticle->AddForce(_force);
	}
#pragma endregion

#pragma region ParticleAnchoredSpring
	ParticleAnchoredSpring::ParticleAnchoredSpring
		(Vector3* a_pAnchorPosition, const number a_nSpringConstant, const number a_nRestLength)
		:m_pAnchorPosition(a_pAnchorPosition),
		m_nSpringConstant(a_nSpringConstant),
		m_nRestLength(a_nRestLength)
	{}

	void ParticleAnchoredSpring::UpdateForce(Particle* a_pParticle, const number a_nDeltaTime)
	{
		/// calculate the vector of the spring.
		Vector3 _force(a_pParticle->GetPosition());
		_force -= *m_pAnchorPosition;

		/// calculate the magnitude of the force.
		number _magnitude = _force.Magnitude();
		_magnitude = (m_nRestLength - _magnitude) * m_nSpringConstant;

		/// calculate the final force and apply it.
		_force.Normalize();
		_force *= -_magnitude;
		a_pParticle->AddForce(_force);
	}
#pragma endregion

#pragma region ParticleBungee
	ParticleBungee::ParticleBungee(Particle* a_pOtherParticle, const number a_nSpringConstant, const number a_nRestLength)
		:m_pOtherParticle(a_pOtherParticle), m_nSpringConstant(a_nSpringConstant), m_nRestLength(a_nRestLength)
	{}

	void ParticleBungee::UpdateForce(Particle* a_pParticle, const number a_nDeltaTime)
	{
		/// calculate vector of the spring.
		Vector3 _force(a_pParticle->GetPosition());
		_force -= m_pOtherParticle->GetPosition();

		/// check if bungee is compressed.
		number _magnitude = _force.Magnitude();
		if (_magnitude <= m_nRestLength)
			return;	//go out if thats the case.

		/// calculate the final force and apply it.
		_force.Normalize();
		_force *= -_magnitude;
		a_pParticle->AddForce(_force);
	}
#pragma endregion

#pragma region ParticleBuoyancy
	ParticleBuoyancy::ParticleBuoyancy(const number a_maxDepth, const number a_volume, 
			const number a_liquidHeight, const number a_liquidDensity)
			:m_nMaxDepth(a_maxDepth),
			m_nVolume(a_volume),
			m_nLiquidLevel(a_liquidHeight),
			m_nLiquidDensity(a_liquidDensity)
	{}

	void ParticleBuoyancy::UpdateForce(Particle* a_pParticle, const number a_nDeltaTime)
	{
		/// calculate the submersion depth.
		number _depth = a_pParticle->GetPosition().y;

		/// check if object is out of liquid.
		if (_depth >= m_nLiquidLevel + m_nMaxDepth)
			return;

		Vector3 _force;

		/// check if object is totally submerged.
		if (_depth <= m_nLiquidLevel - m_nMaxDepth)
		{
			_force.y = m_nLiquidDensity * m_nVolume;
		}
		/// otherwise, the object has somepart of it inside water.
		else
		{
			_force.y = m_nLiquidDensity * m_nVolume *
				((_depth - m_nMaxDepth - m_nLiquidLevel) / 2 * m_nMaxDepth);
		}

		/// apply force.
		a_pParticle->AddForce(_force);
	}
#pragma endregion

#pragma region ParticleFakeSpring
	ParticleFakeSpring::ParticleFakeSpring(Vector3* a_pAnchorPosition, const number a_nSpringConstant, const number a_nDamping)
		:m_pAnchorPosition(a_pAnchorPosition), m_nSpringConstatnt(a_nSpringConstant), m_nDamping(a_nDamping)
	{}

	void ParticleFakeSpring::UpdateForce(Particle* a_pParticle, const number a_nDeltaTime)
	{
		/// make sure the particle doesn't have infinite mass.
		if (!a_pParticle->HasFiniteMass())
			return;

		Vector3 _position = a_pParticle->GetPosition();
		_position -= *m_pAnchorPosition;

		/// calculate the constants and check if they are in bounds.
		number _gamma = 0.5f * num_sqrt(4 * m_nSpringConstatnt - m_nDamping*m_nDamping);
		if (_gamma == 0)
			return;

		Vector3 _c = (_position * (m_nDamping / (2.0f * _gamma))) +
					(a_pParticle->GetVelocity() * ( 1.0f / _gamma));

		/// calculate the target position.
		Vector3 _tagetPos = (a_pParticle->GetPosition() + cos(_gamma + a_nDeltaTime)) +
							(_c * sin(_gamma * a_nDeltaTime));
		
		_tagetPos *= exp(-0.5f * a_nDeltaTime * m_nDamping);

		/// calculate the resulting acceleration, and thereforce the force.
		Vector3 _acc = (_tagetPos - _position) * (1.0f / a_nDeltaTime*a_nDeltaTime) -
					(a_pParticle->GetVelocity() * a_nDeltaTime);
		a_pParticle->AddForce(_acc * a_pParticle->GetMass());
	}
#pragma endregion
}

/// enable the warning back again.
#pragma warning( pop )