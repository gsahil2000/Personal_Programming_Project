#include "World.h"

namespace RocketFrog
{
	void World::StartFrame()
	{
		for (RigidBodyList::iterator _it = m_pRigidBodies.begin(); _it != m_pRigidBodies.end(); ++_it)
		{
			(*_it)->ClearAccumulators();
			(*_it)->CalculateDerivedData();
		}
	}

	void World::RunPhysics(const number a_nDeltaTime)
	{
		/// Update all the forces.
		//m_registry.UpdateForces(a_nDeltaTime);

		/// integrate the objects.
		Integrate(a_nDeltaTime);
	}

	void World::Integrate(const number a_nDeltaTime)
	{
		for (RigidBodyList::iterator _it = m_pRigidBodies.begin(); _it != m_pRigidBodies.end(); ++_it)
		{
			(*_it)->Integrate(a_nDeltaTime);
		}
	}
}