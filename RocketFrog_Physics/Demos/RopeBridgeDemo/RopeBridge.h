#pragma once
#include <ForwardDeclaration.h>
#include <Application.h>
#include <Timer.h>

#include <stdio.h>
#include <cassert>

#define ROD_COUNT	  6
#define CABLE_COUNT	  10
#define SUPPORT_COUNT 12

#define BASE_MASS  1
#define EXTRA_MASS 10

namespace RocketFrog
{
	/// @class RopeBridgeDemo
	/// This class will demonstrate the rope physics.
	class RopeBridgeDemo : public MassAggregateApplication
	{
	public:
		RopeBridgeDemo();
		virtual ~RopeBridgeDemo();

		virtual const char* GetTitle();
		virtual void        Display();
		virtual void        Update();
		virtual void        Key(const unsigned char a_cKey);

	private:
		void UpdateAdditionalMass();

		ParticleCableConstaint* m_pSupports;
		ParticleCable*          m_pCables;
		ParticleRod*            m_pRods;

		Vector3                 m_vMassPos;
		Vector3                 m_vMassDisplayPos;
	};
}