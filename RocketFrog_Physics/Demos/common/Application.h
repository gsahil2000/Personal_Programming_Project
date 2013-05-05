#pragma once

/// @file Application.h
/// Declaration of Application class.

namespace RocketFrog
{
	/// @class Application
	/// Application class provides easy access to create a window for each demo and wraps glut functions.
	class Application
	{
	public:
		Application();										///< Default Ctor.
		Application(const int height, const int width);		///< ctor to set window size.
		virtual ~Application()=0;							///< Dtor.

		void Resize(const int width,const int height);		///< called by OpenGL when the window size is changed.
		virtual void Key(const unsigned char a_key);		///< called on Key press with ASCII code of the key.
		virtual void MouseDrag(const int x, const int y);	///< called on mouse drag.
		
		///< called on Mouse Press
		virtual void Mouse(const int a_key, const int a_state, const int a_x, const int a_y);
		
		
		/// renders the given text at given position of the screen space.
		void RenderText(const float x, const float y, char* const txt, void* font=nullptr);
		
		int GetWidth() const;					///< returns current width of the window
		int GetHieght() const;					///< returns current height of the window

		virtual const char* GetTitle();			///< returns the Title of the current Demo.
		virtual void InitGraphics();			///< Acquires graphical resources for the demo.
		virtual void SetView();					///< Called to set the projection characteristics of the camera.
		virtual void DeInit();					///< Called by Dtor to free up any resources allocated by the application.
		virtual void Display();					///< Called each frame to update display of the current scene.
		virtual void Update();					///< Called each frame to update current state of the scene.

		int m_windowID;
	protected:
		int m_width;		///< holds window width.
		int m_height;		///< holds window height.

		int m_lastMouseX;	///< holds last mouse position X.
		int m_lastMouseY;	///< holds last mouse position Y.
	};
}