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
			number a_damp, number a_mass);

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
		number GetDamping() const;
		void	SetDamping(const number a_damping);

		/// Mass accessors
		number  GetMass() const;
		float	GetInverseMass() const;
		void	SetMass(const number a_mass);
		void	SetInverseMass(const number a_inverseMass);

		/// returns if this particle has finite mass.
		bool	HasFiniteMass() const;

		/// called every frame to update the particle physics state.
		void Integrate(number a_DeltaTime);

		/// adds forces to apply to particle at once.
		void AddForce(const Vector3& a_vForce);

		/// clears the forces acting on this particle.
		void ClearForces();

	protected:
		/// holds the position of the particle in world space.
		Vector3 m_position;

		/// holds the linear velocity of the particle in world space.
		Vector3 m_velocity;

		/// holds the acceleration of the particle.
		Vector3 m_acceleration;

		/// holds the value to reduce the velocity over time.
		number m_damping;

		/// holds inverse mass of the particle
		number m_inverseMass;

		/// this vector accumulates all the forces acting upon this particle.
		Vector3 m_totalForces;
	};
}