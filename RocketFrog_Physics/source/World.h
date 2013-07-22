#pragma once
#include "Rigidbody.h"
#include <vector>

namespace RocketFrog
{
	class World
	{
	public:
		typedef std::vector<Rigidbody*> RigidBodyList;

		/// initializes the world for simulation frame. clears all previous forces.
		void StartFrame();

		/// calls all contactGenerators to register their contacts and returns total number of contacts.
		//unsigned int GenerateContacts();

		/// Integrates all particles in this world.
		void Integrate(const number a_nDeltaTime);

		/// processes all the physics for the particle world.
		void RunPhysics(const number a_nDeltaTime);

		RigidBodyList* GetRigidBodyList();

	protected:
		RigidBodyList m_pRigidBodies;
	};
}