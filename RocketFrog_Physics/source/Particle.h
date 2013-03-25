#pragma once
#include "Vector3.h"

/// @file Particle.h
/// Declaration of class Particle.

namespace RocketFrog
{
	/// @class Particle
	/// class to represent particle in World Space. Particle is 
	/// the simplest object that can be simulated in physics system.
	class Particle
	{
	public:
		/// default Ctor.
		Particle();

		/// Ctor to set all the values.
		Particle(const Vector3& a_pos, const Vector3& a_vel, const Vector3& a_acc,
			float_t a_damp, float_t a_mass);

		/// Dtor.
		~Particle();

		/// position accessors
		Vector3 GetPosition() const;
		void	SetPosition(const Vector3& a_pos);

		/// velocity accessors
		Vector3 GetVelocity() const;
		void	SetVelocity(const Vector3& a_vel);

		/// acceleration accessors
		Vector3 GetAcceleration() const;
		void	SetAcceleration(const Vector3& a_acc);

		/// damping accessors
		float_t GetDamping() const;
		void	SetDamping(const float_t a_damping);

		/// Mass accessors
		float_t GetMass() const;
		float	GetInverseMass() const;
		void	SetMass(const float_t a_mass);
		void	SetInverseMass(const float_t a_inverseMass);

		/// called every frame to update the particle physics state.
		void Integrate(float_t a_DeltaTime);

	protected:
		/// holds the position of the particle in world space.
		Vector3 m_position;

		/// holds the linear velocity of the particle in world space.
		Vector3 m_velocity;

		/// holds the acceleration of the particle.
		Vector3 m_acceleration;

		/// holds the value to reduce the velocity over time.
		float_t m_damping;

		/// holds inverse mass of the particle
		float_t m_inverseMass;
	};
}