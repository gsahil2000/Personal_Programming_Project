#include <Windows.h>
#include <gl\glut.h>
#include "Balllistic.h"

namespace RocketFrog
{
	/// Main uses this to get handle to this class to make a demo.
	Application* GetApplication()
	{
		return new BallisticDemo();
	}

	BallisticDemo::BallisticDemo()
		:m_currentShotType(ShotType::LASER)
	{
		/// clear all shots
		for (AmmoRound *_shot = m_ammo; _shot < m_ammo+m_maxAmmo; ++_shot)
		{
			_shot->m_shotType = ShotType::NONE;
		}
	}

	BallisticDemo::~BallisticDemo()
	{}

	const char* BallisticDemo::GetTitle()
	{
		return "RocketFrogPhysics Ballistics Demo - 31/3/2013";
	}

	void BallisticDemo::Fire()
	{
		AmmoRound* _shot = nullptr;
		for (_shot = m_ammo; _shot < m_ammo+m_maxAmmo; ++_shot)
		{
			if (_shot->m_shotType == ShotType::NONE)
			{
				break;
			}
		}

		/// if no round is available then return.
		if (_shot >= m_ammo + m_maxAmmo)
			return;

		/// set the properties
		switch (m_currentShotType)
		{
		case ShotType::PISTOL:
			_shot->m_particle.SetMass(2.0f);
			_shot->m_particle.SetVelocity(Vector3(0.0f, 0.0f, 35.0f));
			_shot->m_particle.SetAcceleration(Vector3(0.0f, -1.0f, 0.0f));
			_shot->m_particle.SetDamping(0.99f);
			break;
		case ShotType::ARTILLERY:
			_shot->m_particle.SetMass(200.0f);
			_shot->m_particle.SetVelocity(Vector3(0.0f, 30.0f, 40.0f));
			_shot->m_particle.SetAcceleration(Vector3(0.0f, -20.0f, 0.0f));
			_shot->m_particle.SetDamping(0.9f);
			break;
		case ShotType::FIREBALL:
			_shot->m_particle.SetMass(1.0f);
			_shot->m_particle.SetVelocity(Vector3(0.0f, 0.0f, 10.0f));
			_shot->m_particle.SetAcceleration(Vector3(0.0f, 0.6f, 0.0f));
			_shot->m_particle.SetDamping(0.9f);
			break;
		case ShotType::LASER:
			/// This laser is similar to unrealistic laser seen in movies.
			_shot->m_particle.SetMass(0.1f);
			_shot->m_particle.SetVelocity(Vector3(0.0f, 0.0f, 100.0f));
			_shot->m_particle.SetAcceleration(Vector3(0.0f, 0.0f, 0.0f));
			_shot->m_particle.SetDamping(0.99f);
			break;
		default:
			break;
		}

		/// set the data common to all particle types
		_shot->m_particle.SetPosition(Vector3(0.0f,1.5f,0.0f));
		_shot->m_startTime	= Timer::GetInstance()->m_lastFrameTimeStamp;
		_shot->m_shotType	= m_currentShotType;

		/// clear the force accumlators
		_shot->m_particle.ClearForces();
	}

	void BallisticDemo::Update()
	{
		float _duration = (float)Timer::GetInstance()->m_lastFrameDuration * 0.001f;
		if (_duration <= 0.0f)
		{
			return;
		}

		for (AmmoRound* _shot = m_ammo; _shot < m_ammo+m_maxAmmo; ++_shot)
		{
			if (_shot->m_shotType != ShotType::NONE)
			{
				/// Update Physics
				_shot->m_particle.Integrate(_duration);

				/// check if the particle is valid
				/// position should be y > 0 and z < 200
				/// and life time should be less than 5 seconds.
				if (_shot->m_particle.GetPosition().y < 0.0f ||
					_shot->m_startTime+5000 < Timer::GetInstance()->m_lastFrameTimeStamp ||
					_shot->m_particle.GetPosition().z > 200.0f)
				{
					/// if invalid then mark it none
					_shot->m_shotType = ShotType::NONE;
				}
			}
		}

		Application::Update();
	}

	void BallisticDemo::Display()
	{
		// clear the viewport and set camera direction.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(-25.0, 8.0, 5.0, 0.0, 5.0, 22.0, 0.0, 1.0, 0.0);

		// Draw a sphere at the firing point, and add a shadow projected
		// onto the ground plane.
		glColor3f(0.0f, 0.0f, 0.0f);
		glPushMatrix();
		glTranslatef(0.0f, 1.5f, 0.0f);
		glutSolidSphere(0.1f, 5, 5);
		glTranslatef(0.0f, -1.5f, 0.0f);
		glColor3f(0.75f, 0.75f, 0.75f);
		glScalef(1.0f, 0.1f, 1.0f);
		glutSolidSphere(0.1f, 5, 5);
		glPopMatrix();

		// Draw some scale lines
		glColor3f(0.75f, 0.75f, 0.75f);
		glBegin(GL_LINES);
			for (unsigned i = 0; i < 200; i += 10)
			{
				glVertex3f(-5.0f, 0.0f, i);
				glVertex3f(5.0f, 0.0f, i);
			}
		glEnd();

		// Render each particle in turn
		for (AmmoRound* _shot = m_ammo; _shot < m_ammo+m_maxAmmo; ++_shot)
		{
			if (_shot->m_shotType != ShotType::NONE)
			{
				_shot->Render();
			}
		}

		/// render scene description
		glColor3f(0.0f, 0.0f, 0.0f);
		RenderText(10.0, 34.0, "Click: Fire\n1-4: Shot Types");

		/// render current shotType info
		switch (m_currentShotType)
		{
		case RocketFrog::BallisticDemo::NONE:
			RenderText(10.0f, 10.0f, "Current ShotType: None");
			break;
		case RocketFrog::BallisticDemo::PISTOL:
			RenderText(10.0f, 10.0f, "Current ShotType: Pistol");
			break;
		case RocketFrog::BallisticDemo::ARTILLERY:
			RenderText(10.0f, 10.0f, "Current ShotType: Artillery");
			break;
		case RocketFrog::BallisticDemo::FIREBALL:
			RenderText(10.0f, 10.0f, "Current ShotType: Fireball");
			break;
		case RocketFrog::BallisticDemo::LASER:
			RenderText(10.0f, 10.0f, "Current ShotType: Laser");
			break;
		case RocketFrog::BallisticDemo::END:
			break;
		default:
			break;
		}
	}

	void BallisticDemo::Mouse(const int a_key, const int a_state, const int a_x, const int a_y)
	{
		/// fire weapon
		if (a_state == GLUT_DOWN)
		{
			Fire();
		}
	}

	void BallisticDemo::Key(const unsigned char a_key)
	{
		switch (a_key)
		{
		case '1':
			m_currentShotType = ShotType::PISTOL;
			break;
		case '2':
			m_currentShotType = ShotType::ARTILLERY;
			break;
		case '3':
			m_currentShotType = ShotType::FIREBALL;
			break;
		case '4':
			m_currentShotType = ShotType::LASER;
			break;
		case 27:	/// Esc key
			glutDestroyWindow(m_windowID);
			exit(0);
			break;
		default:
			break;
		}
	}

	/*----------------------AmmoRound Function---------------------*/
	void BallisticDemo::AmmoRound::Render()
	{
		Vector3 _pos = m_particle.GetPosition();

		/// render the bullet sphere itself.
		switch (m_shotType)
		{
		case RocketFrog::BallisticDemo::NONE:
			break;
		case RocketFrog::BallisticDemo::PISTOL:
			glColor3f(0, 0, 0);
			break;
		case RocketFrog::BallisticDemo::ARTILLERY:
			glColor3f(1, 0, 0);
			break;
		case RocketFrog::BallisticDemo::FIREBALL:
			glColor3f(1, 1, 0);
			break;
		case RocketFrog::BallisticDemo::LASER:
			glColor3f(1, 0, 1);
			break;
		case RocketFrog::BallisticDemo::END:
			break;
		default:
			break;
		}
		
		glPushMatrix();
		glTranslatef(_pos.x, _pos.y, _pos.z);
		glutSolidSphere(0.3f, 5, 4);
		glPopMatrix();

		/// shadow effect by drawing a grey circle.
		glColor3f(0.75, 0.75, 0.75);
		glPushMatrix();
		glTranslatef(_pos.x, 0, _pos.z);
		glScalef(1.0f, 0.1f, 1.0f);
		glutSolidSphere(0.6f, 5, 4);
		glPopMatrix();
	}
}