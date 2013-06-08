#pragma once
#include "ParticleContact.h"

namespace RocketFrog
{
#pragma region ParticleLink
	/// @class ParticleLink
	/// It connects particles and generates contacts if move farther than maxLength.
	class ParticleLink : public ParticleContactGenerator
	{
	public:
		/// holds the pair of particles concected by this link.
		Particle* m_pParticles[2];

	protected:
		/// returns current length of link.
		number GetCurrentLength() const;
	};
#pragma endregion //ParticleLink.

#pragma region ParticleCable
	/// @class ParticleCable
	/// It links a pair of particles, generating a contact if they stray too far apart.
	class ParticleCable : public ParticleLink
	{
	public:
		/// holds the maximum length of the cable.
		number m_nMaxLength;

		/// holds the retitution(bounciness) of the cable.
		number m_nRestitution;

		/// returns 0, if cable not extended otherwise 1.
		virtual unsigned int AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const;
	};
#pragma endregion //ParticleCable

#pragma region ParticleRod
	/// @class ParticleRod
	/// It generates contact if the particles try to move farther or closer from specified range.
	class ParticleRod : public ParticleLink
	{
	public:
		/// holds the length of the rod.
		number m_nLength;

		/// returns 0, if cable not extended otherwise 1.
		virtual unsigned int AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const;
	};
#pragma endregion //ParticleRod

#pragma region ParticleConstraints
	/// ParticleConstraints are link to connect particles with immaovable things.
	class ParticleConstraint : public ParticleContactGenerator
	{
	public:
		Particle* GetParticle();
		void      SetParticle(Particle* a_pParticle);

		Vector3   GetAnchorPosition() const;
		void      SetAnchorPosition(Vector3& a_vAnchorPosition);

		virtual unsigned int AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const = 0;

	protected:
		number CurrentLength() const;

		Particle* m_pParticle;			// holds the particles connected by this constraint.
		Vector3   m_vAnchorPosition;	// immovable point to which the particle is connected.
	};
#pragma endregion //ParticleEndConstraints.

#pragma region ParticleCableConstraint
	class ParticleCableConstaint : public ParticleConstraint
	{
	public:
		number GetMaxLength() const;
		void   SetMaxLenth(number a_nMaxLength);

		number GetRestitution() const;
		void   SetRestitution(number a_nRestitution);

		// from ParentClass.
		unsigned int AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const;

	private:
		number m_nMaxLength;
		number m_nRestitution;
	};
#pragma endregion //ParticleCableConstratint

#pragma region PartilceRodConstraint
	class ParticleRodConstraint : public ParticleConstraint
	{
	public:
		number GetLength() const;
		void   SetLength(number a_nLength);

		// from parent class.
		unsigned int AddContact(ParticleContact* a_pContact, const unsigned int a_nLimit) const;

	private:
		number m_nLength;
	};
#pragma endregion //ParticleRodConstraint
}