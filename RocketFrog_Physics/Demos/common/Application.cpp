#include <Windows.h>
#include <gl\glut.h>
#include <string>
#include "Application.h"
#include "Timer.h"

namespace RocketFrog
{
	Application::Application()
		:m_height(1024), m_width(768), m_lastMouseX(0), m_lastMouseY(0)
	{}

	Application::Application(const int height, const int width)
		:m_height(height), m_width(width), m_lastMouseX(0), m_lastMouseY(0)
	{}

	Application::~Application()
	{
		DeInit();
	}

	void Application::InitGraphics()
	{
		glClearColor(0.9f, 0.95f, 1.0f, 1.0f);	///< clear the background color to light gray
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		SetView();
	}

	void Application::SetView()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, (double)m_width/(double)m_height, 1.0, 500.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void Application::Display()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_LINES);
			glVertex2i(1, 1);
			glVertex2i(639, 319);
		glEnd();
	}

	const char* Application::GetTitle()
	{
		return "RocketFrogPhyics Demo";
	}

	void Application::DeInit()
	{}

	void Application::Update()
	{
		glutPostRedisplay();	///< marks that current OpenGL window needs to be redisplayed before next frame.
	}

	void Application::Resize(const int width,const int height)
	{
		m_width = width;

		/// Avoiding divide by zero
		if (height <= 0)
		{
			m_height = 1;
		}
		else
		{
			m_height = height;
		}

		glViewport(0, 0, m_width, m_height);
		SetView();
	}

	void Application::Key(const unsigned char a_key)
	{}

	void Application::Mouse(const int a_key, const int a_state, const int x, const int y)
	{}
	
	void Application::MouseDrag(const int x, const int y)
	{}

	void Application::RenderText(const float x, const float y, char* const txt, void* font)
	{
		int _y = y;	// it may change if text includes new line character.

		/// temporarily disable depth test and setup orthographic projection view.
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, (double)m_width, 0.0, (double)m_height, -1.0, 1.0);

		/// Move to modelview mode.
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		/// make sure we have font
		if (font == nullptr)
		{
			font = GLUT_BITMAP_HELVETICA_10;
		}

		/// loop through characters displaying them
		size_t _len = strlen(txt);

		glRasterPos2f(x, _y);
		for (const char* letter = txt; letter < txt+_len; ++letter)
		{
			/// if letter is new line character,
			/// then move down by the line height/
			if (*letter == '\n')
			{
				_y -= 12.0;
				glRasterPos2f(x, _y);
			}
			glutBitmapCharacter(font, *letter);
		}

		/// Pop the matrices and go back to state before this function.
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_DEPTH_TEST);
	}

#pragma region MassAggregateApplication
	MassAggregateApplication::MassAggregateApplication(const unsigned int a_nParticleCount)
		:m_world(a_nParticleCount*10), m_particleArray(new Particle[a_nParticleCount]), m_groundContactGenerator()
	{
		for (unsigned int i = 0; i < a_nParticleCount; ++i)
		{
			m_world.GetParticleList()->push_back(&m_particleArray[i]);
		}

		m_groundContactGenerator.Init(m_world.GetParticleList());
		m_world.GetContactGeneratorList()->push_back(&m_groundContactGenerator);
	}

	MassAggregateApplication::~MassAggregateApplication()
	{
		delete[] m_particleArray;
	}

	void MassAggregateApplication::InitGraphics()
	{
		// call the base class function.
		Application::InitGraphics();
	}

	void MassAggregateApplication::Display()
	{
		// clear viewport and set the camera direction;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(0.0, 3.5, 8.0,  0.0, 3.5, 0.0,  0.0, 1.0, 0.0);

		glColor3f(0, 0, 0);

		ParticleWorld::ParticleList* _particleList = m_world.GetParticleList();
		ParticleWorld::ParticleList::iterator _it  = _particleList->begin();

		for (; _it != _particleList->end(); ++_it)
		{
			Particle* _particle = *_it;
			Vector3&  _pos      = _particle->GetPosition();
			glPushMatrix();
			glTranslatef(_pos.x, _pos.y, _pos.z);
			glutSolidSphere(0.1f, 20, 10);
			glPopMatrix();
		}
	}

	void MassAggregateApplication::Update()
	{
		// Clear Accumators.
		m_world.StartFrame();

		// Find the duration of the last frame in seconds.
		float _duration = (float)Timer::GetInstance()->m_lastFrameDuration * 0.001f;
		if (_duration <= 0.0f)
		{
			return;
		}
		
		// Run the Simulation.
		m_world.RunPhysics(_duration);

		Application::Update();
	}
#pragma endregion //MassAggregateApplication
}