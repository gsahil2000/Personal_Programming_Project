#include <Windows.h>
#include <gl/glut.h>
#include "RopeBridge.h"

namespace RocketFrog
{
	/// Main uses this to get handle to this class to make a demo.
	Application* GetApplication()
	{
		return new RopeBridgeDemo();
	}

	RopeBridgeDemo::RopeBridgeDemo()
		:MassAggregateApplication(12), 
		m_pSupports(nullptr), m_pCables(nullptr), m_pRods(nullptr),
		m_vMassPos(0.0f, 0.0f, 0.5f)
	{
		// generate masses and connections.
		for (unsigned int i = 0; i < 12; ++i)
		{
			unsigned int x = (i % 12) / 2;
			m_particleArray[i].SetPosition(Vector3(number(i/2)*2.0f-5.0f, 4, number(i%2)*2.0f-1.0f));
			m_particleArray[i].SetVelocity(Vector3(0,0,0));
			m_particleArray[i].SetDamping(0.9f);
			m_particleArray[i].SetAcceleration(Vector3::GRAVITY);
			m_particleArray[i].ClearForces();
		}

		// links
		m_pCables = new ParticleCable[CABLE_COUNT];
		for (unsigned int i = 0; i < CABLE_COUNT; ++i)
		{
			m_pCables[i].m_pParticles[0] = &m_particleArray[i];
			m_pCables[i].m_pParticles[1] = &m_particleArray[i+2];
			m_pCables[i].m_nMaxLength = 1.9f;
			m_pCables[i].m_nRestitution = 0.3f;
			m_world.GetContactGeneratorList()->push_back(&m_pCables[i]);
		}

		m_pSupports = new ParticleCableConstaint[SUPPORT_COUNT];
		for (unsigned int i = 0; i < SUPPORT_COUNT; ++i)
		{
			m_pSupports[i].SetParticle(m_particleArray+i);
			m_pSupports[i].SetAnchorPosition(Vector3(number(i/2)*2.2f-5.5f, 6, number(i%2)*1.6f-0.8f));

			if (i < 6)
			{
				m_pSupports[i].SetMaxLenth(number(i/2)*0.5f + 3.0f);
			}
			else
			{
				m_pSupports[i].SetMaxLenth(5.5f - number(i/2)*0.5f);
			}

			m_pSupports[i].SetRestitution(0.5f);
			m_world.GetContactGeneratorList()->push_back(&m_pSupports[i]);
		}

		// rods
		m_pRods = new ParticleRod[ROD_COUNT];
		for (unsigned int i = 0; i < ROD_COUNT; ++i)
		{
			m_pRods[i].m_pParticles[0] = &m_particleArray[i*2];
			m_pRods[i].m_pParticles[1] = &m_particleArray[i*2 + 1];
			m_pRods[i].m_nLength = 2;
			m_world.GetContactGeneratorList()->push_back(&m_pRods[i]);
		}

		UpdateAdditionalMass();
	}

	RopeBridgeDemo::~RopeBridgeDemo()
	{
		if (m_pCables)
		{
			delete[] m_pCables;
		}

		if (m_pRods)
		{
			delete[] m_pRods;
		}

		if (m_pSupports)
		{
			delete[] m_pSupports;
		}
	}

	void RopeBridgeDemo::UpdateAdditionalMass()
	{
		for (unsigned int i = 0; i < 12; ++i)
		{
			m_particleArray[i].SetMass(BASE_MASS);
		}

		// find coordinates of mass as Index and proportion.
		int _x     = int(m_vMassPos.x);
		number _xp = fmod(m_vMassPos.z, number(1.0f));
		if (_x < 0)
		{
			_x  = 0;
			_xp = 0;
		}
		if (_x >= 5)
		{
			_x  = 5;
			_xp = 0;
		}

		int _z     = int(m_vMassPos.z);
		number _zp = fmod(m_vMassPos.z, number(1.0f));
		if (_z < 0)
		{
			_z  = 0;
			_zp = 0;
		}
		if (_z >= 1)
		{
			_z  = 1;
			_zp = 0;
		}

		// calculate where to draw the mass.
		m_vMassDisplayPos.Clear();

		// Add proportion to correct masses.
		m_particleArray[_x * 2 + _z].SetMass(BASE_MASS + EXTRA_MASS * (1 - _xp) * (1 - _zp));
		m_vMassDisplayPos.AddScaledVector(m_particleArray[_x * 2 + _z].GetPosition(), (1 - _xp) * (1 - _zp));

		if (_xp > 0)
		{
			m_particleArray[_x*2 + _z + 2].SetMass(BASE_MASS + EXTRA_MASS * _xp * (1 - _zp));
			m_vMassDisplayPos.AddScaledVector(m_particleArray[_x*2 + _z + 2].GetPosition(), _xp*(1 - _zp));

			if (_zp > 0)
			{
				m_particleArray[_x*2 + _z + 3].SetMass(BASE_MASS + EXTRA_MASS * _xp * _zp);
				m_vMassDisplayPos.AddScaledVector(m_particleArray[_x*2 + _z + 3].GetPosition(), _xp*_zp);
			}
		}

		if (_zp > 0)
		{
			m_particleArray[_x*2 + _z + 1].SetMass(BASE_MASS + EXTRA_MASS * (1 - _xp) * _zp);
			m_vMassDisplayPos.AddScaledVector(m_particleArray[_x*2 + _z + 1].GetPosition(), (1 - _xp)*_zp);
		}
	}

	void RopeBridgeDemo::Display()
	{
		MassAggregateApplication::Display();

		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		for (unsigned int i = 0; i < ROD_COUNT; ++i)
		{
			Particle** _particles = m_pRods[i].m_pParticles;
			const Vector3& _p0    = _particles[0]->GetPosition();
			const Vector3& _p1    = _particles[1]->GetPosition();
			glVertex3f(_p0.x, _p0.y, _p0.z);
			glVertex3f(_p1.x, _p1.y, _p1.z);
		}

		glColor3f(0, 1, 0);
		for (unsigned int i = 0; i < CABLE_COUNT; ++i)
		{
			Particle** _particles = m_pCables[i].m_pParticles;
			const Vector3& _p0    = _particles[0]->GetPosition();
			const Vector3& _p1    = _particles[1]->GetPosition();
			glVertex3f(_p0.x, _p0.y, _p0.z);
			glVertex3f(_p1.x, _p1.y, _p1.z);
		}

		glColor3f(0.7f, 0.7f, 0.7f);
		for (unsigned int i = 0; i < SUPPORT_COUNT; ++i)
		{
			const Vector3& _p0 = m_pSupports[i].GetParticle()->GetPosition();
			const Vector3& _p1 = m_pSupports[i].GetAnchorPosition();
			glVertex3f(_p0.x, _p0.y, _p0.z);
			glVertex3f(_p1.x, _p1.y, _p1.z);
		}
		glEnd();

		glColor3f(1, 0, 0);
		glPushMatrix();
		glTranslatef(m_vMassDisplayPos.x, m_vMassDisplayPos.y+0.25f, m_vMassDisplayPos.z);
		glutSolidSphere(0.25f, 20, 10);
		glPopMatrix();
	}

	void RopeBridgeDemo::Update()
	{
		MassAggregateApplication::Update();
		UpdateAdditionalMass();
	}

	const char* RopeBridgeDemo::GetTitle()
	{
		return "RocketFrogPhysics: RopeBridgeDemo 7/6/2013";
	}

	void RopeBridgeDemo::Key(const unsigned char a_cKey)
	{
		switch (a_cKey)
		{
		case 'w': case 'W':
			m_vMassPos.z -= 0.1f;
			if (m_vMassPos.z < 0.0f)
			{
				m_vMassPos.z = 0.0f;
			}
			break;

		case 's': case 'S':
			m_vMassPos.z += 0.1f;
			if (m_vMassPos.z > 1.0f)
			{
				m_vMassPos.z = 1.0f;
			}
			break;

		case 'a': case 'A':
			m_vMassPos.x -= 0.1f;
			if (m_vMassPos.x < 0.0f)
			{
				m_vMassPos.x = 0.0f;
			}
			break;

		case 'd': case 'D':
			m_vMassPos.x += 0.1f;
			if (m_vMassPos.x > 5.0f)
			{
				m_vMassPos.x = 5.0f;
			}
			break;

		default:
			MassAggregateApplication::Key(a_cKey);
		}
	}
}