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
}