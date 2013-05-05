#pragma once
#include "Particle.h"

namespace RocketFrog
{
	/// forward declaration.
	class ParticleContactResolution;

	class ParticleContact
	{
		/**
         * The contact resolver object needs access into the contacts to
         * set and effect the contact.
         */
        friend ParticleContactResolution;

	public:
		/// Particle accessors
		Particle* GetParticle(const unsigned short a_nIndex) const;
		void SetParticle(Particle* const a_pParticle, const unsigned short a_nIndex);

		/// restitution coefficient accessors.
		number GetRestitutionCoefficient() const;
		void SetRestitutionCoefficient(const number a_nValue);

		/// ContactNormal accessors.
		Vector3 GetContactNormal() const;
		void SetContactNormal(const Vector3& a_vContactNormal);

		/// penetration accessors.
		number GetContactPenetration() const;
		void SetContactPenetration(const number a_nPenetration);

	protected:
		/// called to resolve this collision with both velocity and penetration.
		void Resolve(const number a_nDeltaTime);

		/// calculates the separating velocity at this collision.
		number CalculateSeparatingVelocity() const;

	private:
		/// handles the impulse calculations for this collision.
		void ResolveVelocity(const number a_nDeltaTime);

		/// handles the interpenetration resolution of this contact.
		void ResolveInterPenetration(/*const number a_nDeltaTime*/);

		/// holds the both the particles involved in this collision.
		Particle* m_pParticles[2];

		/// holds the normal restitution coefficient at the contact.
		number m_nRestitutionCoeff;

		/// holds the direction of contact in world coordination system.
		Vector3 m_vContactNormal;

		/// holds the depth of penetration of the contact.
		number m_nPenetration;

		/// holds the how much each particle has to move.
		Vector3 m_vParticleMovement[2];
	};
	
	
    /// @class ParticleContactGenerator
    /// basic polymorphic interface for contact generators applying to particles.
    class ParticleContactGenerator
    {
    public:
        /**
         * Fills the given contact structure with the generated
         * contact. The contact pointer should point to the first
         * available contact in a contact array, where limit is the
         * maximum number of contacts in the array that can be written
         * to. The method returns the number of contacts that have
         * been written.
         */
        virtual unsigned int AddContact(ParticleContact* a_pContact,
                                    const unsigned int a_nLimit) const = 0;
    };
}