#include <Windows.h>
#include <gl\glut.h>
#include "FlightSimDemo.h"

namespace RocketFrog
{
	/// Main uses this to get handle to this class to make a demo.
	Application* GetApplication()
	{
		return new FlightSimDemo();
	}

	FlightSimDemo::FlightSimDemo()
		:Application(), 
		m_rightWing(Matrix3x3(0,0,0, -1,-0.5f,0, 0,0,0),
					Matrix3x3(0,0,0, -0.995f,-0.5f,0, 0,0,0),
					Matrix3x3(0,0,0, -1.005f,-0.5f,0, 0,0,0),
					Vector3(-1.0f, 0.0f, 2.0f), &m_vWindSpeed),

		m_leftWing(Matrix3x3(0,0,0, -1,-0.5f,0, 0,0,0),
				   Matrix3x3(0,0,0, -0.995f,-0.5f,0, 0,0,0),
				   Matrix3x3(0,0,0, -1.005f,-0.5f,0, 0,0,0),
				   Vector3(-1.0f, 0.0f, -2.0f), &m_vWindSpeed),

		m_rudder(Matrix3x3(0,0,0, 0,0,0, 0,0,0),
				 Matrix3x3(0,0,0, 0,0,0, 0.01f,0,0),
				 Matrix3x3(0,0,0, 0,0,0, -0.01f,0,0),
				 Vector3(2.0f, 0.5f, 0), &m_vWindSpeed),

		m_tail(Matrix3x3(0,0,0, -1,-0.5f,0, 0,0,-0.1f),
			   Vector3(2.0f, 0, 0), &m_vWindSpeed),

	    m_fLeftWingControl(0), m_fRightWingControl(0), m_fRudderControl(0),
		m_vWindSpeed(0,0,0)
	{
		// setup aircraft rigid body.
		ResetPlane();

		m_aircraft.SetMass(2.5f);
		Matrix3x3 _it;
		_it.SetBlockInertiaTensor(Vector3(2, 1, 1), 1);
		m_aircraft.SetInertiaTensor(_it);

		m_aircraft.SetDamping(0.8f);
		m_aircraft.SetAngularDamping(0.8f);
		
		m_aircraft.SetAcceleration(Vector3::GRAVITY);
		m_aircraft.CalculateDerivedData();

		m_aircraft.SetAwake(true);
		m_aircraft.SetCanSleep(false);

		m_registry.Add(&m_aircraft, &m_leftWing);
		m_registry.Add(&m_aircraft, &m_rightWing);
		m_registry.Add(&m_aircraft, &m_rudder);
		m_registry.Add(&m_aircraft, &m_tail);
	}

	FlightSimDemo::~FlightSimDemo()
	{
		m_registry.Clear();
	}

	void FlightSimDemo::ResetPlane()
	{
		m_aircraft.SetPosition(Vector3(0, 0, 0));
		m_aircraft.SetOrientation(Quaternion(1, 0, 0, 0));

		m_aircraft.SetVelocity(Vector3(0, 0, 0));
		m_aircraft.SetRotation(Vector3(0, 0, 0));
	}

	static void DrawAircraft()
	{
		// Fuselage
		glPushMatrix();
		glTranslatef(-0.5f, 0, 0);
		glScalef(2.0f, 0.8f, 1.0f);
		glutSolidCube(1.0f);
		glPopMatrix();

		// Rear Fuselage
		glPushMatrix();
		glTranslatef(1.0f, 0.15f, 0);
		glScalef(2.75f, 0.5f, 0.5f);
		glutSolidCube(1.0f);
		glPopMatrix();

		// Wings
		glPushMatrix();
		glTranslatef(0, 0.3f, 0);
		glScalef(0.8f, 0.1f, 6.0f);
		glutSolidCube(1.0f);
		glPopMatrix();

		// Rudder
		glPushMatrix();
		glTranslatef(2.0f, 0.775f, 0);
		glScalef(0.75f, 1.15f, 0.1f);
		glutSolidCube(1.0f);
		glPopMatrix();

		// Tail-plane
		glPushMatrix();
		glTranslatef(1.9f, 0, 0);
		glScalef(0.85f, 0.1f, 2.0f);
		glutSolidCube(1.0f);
		glPopMatrix();
	}

	void FlightSimDemo::Display()
	{
		// Clear the view port and set the camera direction
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		Vector3 pos = m_aircraft.GetPosition();
		Vector3 offset(4.0f+m_aircraft.GetVelocity().Magnitude(), 0, 0);
		offset = m_aircraft.GetTransformMatrix().TransformDirection(offset);
		gluLookAt(pos.x+offset.x, pos.y+5.0f, pos.z+offset.z,
				  pos.x, pos.y, pos.z,
				  0.0, 1.0, 0.0);

		glColor3f(0.6f,0.6f,0.6f);
		int bx = int(pos.x);
		int bz = int(pos.z);
		glBegin(GL_QUADS);
		for (int x = -20; x <= 20; x++) for (int z = -20; z <= 20; z++)
		{
			glVertex3f(bx+x-0.1f, 0, bz+z-0.1f);
			glVertex3f(bx+x-0.1f, 0, bz+z+0.1f);
			glVertex3f(bx+x+0.1f, 0, bz+z+0.1f);
			glVertex3f(bx+x+0.1f, 0, bz+z-0.1f);
		}
		glEnd();

		// Set the transform matrix for the aircraft
		Matrix3x4 transform = m_aircraft.GetTransformMatrix();
		GLfloat gl_transform[16];
		transform.FillGLArray(gl_transform);
		glPushMatrix();
		glMultMatrixf(gl_transform);

		// Draw the aircraft
		glColor3f(0,0,0);
		DrawAircraft();
		glPopMatrix();

		glColor3f(0.8f, 0.8f, 0.8f);
		glPushMatrix();
		glTranslatef(0, -1.0f - pos.y, 0);
		glScalef(1.0f, 0.001f, 1.0f);
		glMultMatrixf(gl_transform);
		DrawAircraft();
		glPopMatrix();

		char buffer[256];
		sprintf_s(
			buffer,
			"Altitude: %.1f | Speed %.1f",
			m_aircraft.GetPosition().y,
			m_aircraft.GetVelocity().Magnitude()
			);
		glColor3f(0,0,0);
		RenderText(10.0f, 24.0f, buffer);

		sprintf_s(
			buffer,
			"Left Wing: %.1f | Right Wing: %.1f | Rudder %.1f",
			m_fLeftWingControl, m_fRightWingControl, m_fRudderControl
			);
		RenderText(10.0f, 10.0f, buffer);
	}

	void FlightSimDemo::Update()
	{
		// Find the duration of the last frame in seconds
		float duration = (float)Timer::GetInstance()->m_lastFrameDuration * 0.001f;
		if (duration <= 0.0f) return;

		// Start with no forces or acceleration.
		m_aircraft.ClearAccumulators();

		// Add the propeller force
		Vector3 propulsion(-10.0f, 0, 0);
		propulsion = m_aircraft.GetTransformMatrix().TransformDirection(propulsion);
		m_aircraft.AddForce(propulsion);

		// Add the forces acting on the aircraft.
		m_registry.UpdateForces(duration);

		// Update the aircraft's physics.
		m_aircraft.Integrate(duration);

		// Do a very basic collision detection and response with the ground.
		Vector3 pos = m_aircraft.GetPosition();
		if (pos.y < 0.0f)
		{
			pos.y = 0.0f;
			m_aircraft.SetPosition(pos);

			if (m_aircraft.GetVelocity().y < -10.0f)
			{
				ResetPlane();
			}
		}

		Application::Update();
	}

	const char* FlightSimDemo::GetTitle()
	{
		return "RocketFrogPhysics: FlightSimDemo 22/7/2013";
	}

	void FlightSimDemo::KeyPress(unsigned char a_cKey)
	{
		switch(a_cKey)
		{
		case 'q': case 'Q':
			m_fRudderControl += 0.1f;
			break;

		case 'e': case 'E':
			m_fRudderControl -= 0.1f;
			break;

		case 'w': case 'W':
			m_fLeftWingControl -= 0.1f;
			m_fRightWingControl -= 0.1f;
			break;

		case 's': case 'S':
			m_fLeftWingControl += 0.1f;
			m_fRightWingControl += 0.1f;
			break;

		case 'd': case 'D':
			m_fLeftWingControl -= 0.1f;
			m_fRightWingControl += 0.1f;
			break;

		case 'a': case 'A':
			m_fLeftWingControl += 0.1f;
			m_fRightWingControl -= 0.1f;
			break;

		case 'x': case 'X':
			m_fLeftWingControl = 0.0f;
			m_fRightWingControl = 0.0f;
			m_fRudderControl = 0.0f;
			break;

		case 'r': case 'R':
			ResetPlane();
			break;

		default:
			Application::KeyPress(a_cKey);
		}

		// Make sure the controls are in range
		if (m_fLeftWingControl < -1.0f) m_fLeftWingControl = -1.0f;
		else if (m_fLeftWingControl > 1.0f) m_fLeftWingControl = 1.0f;
		if (m_fRightWingControl < -1.0f) m_fRightWingControl = -1.0f;
		else if (m_fRightWingControl > 1.0f) m_fRightWingControl = 1.0f;
		if (m_fRudderControl < -1.0f) m_fRudderControl = -1.0f;
		else if (m_fRudderControl > 1.0f) m_fRudderControl = 1.0f;

		// Update the control surfaces
		m_leftWing.SetControl(m_fLeftWingControl);
		m_rightWing.SetControl(m_fRightWingControl);
		m_rudder.SetControl(m_fRudderControl);
	}

	void FlightSimDemo::KeyUp(unsigned char a_cKey)
	{
	}
}