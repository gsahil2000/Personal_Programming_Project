#include <Windows.h>
#include <gl\glut.h>
#include <Timer.h>
#include "../Ballistics/BallisticsDemo/Balllistic.h"

using namespace RocketFrog;

Application* g_app;

void createWindow()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 600);
	glutInitWindowPosition(0, 0);
	g_app->m_windowID = glutCreateWindow(g_app->GetTitle());
}

void Update()
{
	Timer::GetInstance()->Update();
	g_app->Update();
}

void Display()
{
	g_app->Display();

	glFlush();
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y)
{
	g_app->Mouse(button, state, x, y);
}

void Reshape(int width, int height)
{
	g_app->Resize(width, height);
}

void KeyPress(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(EXIT_SUCCESS);
	}
	g_app->KeyPress(key);
}

void KeyRelease(unsigned char a_cKey, int a_x, int a_y)
{
	g_app->KeyUp(a_cKey);
}

void Motion(int x, int y)
{
	g_app->MouseDrag(x, y);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	Timer::Init();

	g_app = GetApplication();
	createWindow();

	/// register callback functions.
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyPress);
	glutKeyboardUpFunc(KeyRelease);
	glutDisplayFunc(Display);
	glutIdleFunc(Update);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	// run the application
	g_app->InitGraphics();
	glutMainLoop();

	/// clean up the application
	g_app->DeInit();
	delete g_app;
	Timer::Destroy();
}