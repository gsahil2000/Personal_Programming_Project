#include "Particle.h"
#include <assert.h>

namespace RocketFrog
{
	Particle::Particle()
		:m_position(), m_velocity(), m_acceleration(),
		m_damping(), m_inverseMass()
	{}

	Particle::Particle(const Vector3& a_pos, const Vector3& a_vel, const Vector3& a_acc,
			number a_damp, number a_mass)
			:m_position(a_pos),
			m_velocity(a_vel),
			m_acceleration(a_acc),
			m_damping(a_damp),
			m_inverseMass(1/a_mass)
	{
		assert(m_inverseMass != 0);
	}

	Particle::~Particle()
	{}

	Vector3 Particle::GetPosition() const
	{
		return m_position;
	}

	void Particle::SetPosition(const Vector3& a_pos)
	{
		m_position = a_pos;
	}

	Vector3 Particle::GetVelocity() const
	{
		return m_velocity;
	}

	void Particle::SetVelocity(const Vector3& a_vel)
	{
		m_velocity = a_vel;
	}

	Vector3 Particle::GetAcceleration() const
	{
		return m_acceleration;
	}

	void Particle::SetAcceleration(const Vector3& a_acc)
	{
		m_acceleration = a_acc;
	}

	number Particle::GetDamping() const
	{
		return m_damping;
	}

	void Particle::SetDamping(number a_damping)
	{
		m_damping = a_damping;
	}

	number Particle::GetMass() const
	{
		if (m_inverseMass == 0)
		{
			return num_max;
		}
		else
		{
			return (number(1.0) / m_inverseMass);
		}
	}

	number Particle::GetInverseMass() const
	{
		return m_inverseMass;
	}

	void Particle::SetMass(float a_mass)
	{
		assert(a_mass != 0);
		m_inverseMass = (number(1.0) / a_mass);
	}

	void Particle::SetInverseMass(float a_inverseMass)
	{
		m_inverseMass = a_inverseMass;
	}

	bool Particle::HasFiniteMass() const
	{
		return m_inverseMass >= 0.0f;
	}

	void Particle::Integrate(number a_DeltaTime)
	{
		/// don't integrate if particle has infinite mass
		if (m_inverseMass <= 0)
			return;

		/// assert the time since last update is greater than 0
		assert(a_DeltaTime > 0);

		/// update position
		m_position.AddScaledVector(m_velocity, a_DeltaTime);

		/// calculate acceleration from force
		Vector3 _result = m_acceleration;
		_result.AddScaledVector(m_totalForces, m_inverseMass);


		/// update the velocity of particle
		m_velocity.AddScaledVector(_result, a_DeltaTime);

		/// impose drag.
		m_velocity *= num_pow(m_damping, a_DeltaTime);
	}

	void Particle::AddForce(const Vector3& a_vForce)
	{
		m_totalForces += a_vForce;
	}

	void Particle::ClearForces()
	{
		m_totalForces.Clear();
	}
}