#pragma once
#include <ForwardDeclaration.h>
#include <Application.h>
#include <Timer.h>

#include <stdio.h>
#include <cassert>

namespace RocketFrog
{
	class FlightSimDemo : public Application
	{
	public:
		/* Creates new demo project. */
		FlightSimDemo();
		virtual ~FlightSimDemo();

		/* Returns the window title for the demo. */
		virtual const char* GetTitle();

		/* Display the particles. */
		virtual void Display();

		/* Update the particle positions. */
		virtual void Update();

		/* Handle a key press. */
		virtual void KeyPress(const unsigned char a_cKey);
		virtual void KeyUp(const unsigned char a_cKey);

	protected:

	private:
		AeroControl   m_leftWing;
		AeroControl   m_rightWing;
		AeroControl   m_rudder;
		Aero          m_tail;
		Rigidbody     m_aircraft;
		ForceRegistry m_registry;

		Vector3 m_vWindSpeed;

		float m_fLeftWingControl;
		float m_fRightWingControl;
		float m_fRudderControl;

		void ResetPlane();
	};
}