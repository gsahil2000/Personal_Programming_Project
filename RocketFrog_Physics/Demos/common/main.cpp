#include <Windows.h>
#include <gl\glut.h>
#include <Timer.h>
#include "../Ballistics/BallisticsDemo/Balllistic.h"

using namespace RocketFrog;

//extern Application* GetApplication();

Application* g_app;

void createWindow(const char* a_title)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 320);
	glutInitWindowPosition(0, 0);
	g_app->m_windowID = glutCreateWindow(a_title);
}

void update()
{
	Timer::GetInstance()->Update();
	g_app->Update();
}

void display()
{
	g_app->Display();

	glFlush();
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	g_app->Mouse(button, state, x, y);
}

void reshape(int width, int height)
{
	g_app->Resize(width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	g_app->Key(key);
}

void motion(int x, int y)
{
	g_app->MouseDrag(x, y);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	Timer::Init();

	g_app = GetApplication();
	createWindow(g_app->GetTitle());

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// run the application
	g_app->InitGraphics();
	glutMainLoop();

	/// clean up the application
	g_app->DeInit();
	delete g_app;
	Timer::Destroy();
}