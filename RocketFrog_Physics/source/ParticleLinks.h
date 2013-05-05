#pragma once
#include "ParticleContact.h"

namespace RocketFrog
{
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
}