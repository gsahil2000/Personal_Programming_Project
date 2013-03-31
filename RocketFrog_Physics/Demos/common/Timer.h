#pragma once

namespace RocketFrog
{
	class Timer
	{
	public:
		unsigned int m_frameNumber;
		unsigned int m_lastFrameTimeStamp;
		unsigned int m_lastFrameDuration;
		
		unsigned long m_lastFrameClockStamp;
		unsigned long m_lastFrameClockTicks;

		bool		  m_isPaused;

		double		  m_averageFrameDuration;

		float		  m_fps;

		static Timer* GetInstance();

		static void Update();

		static void Init();

		static void Destroy();

		static unsigned int GetTime();

		static unsigned long GetClock();

	private:
		Timer();
		Timer(const Timer&);
		Timer& operator=(const Timer&);

		static Timer*		m_instance;
	};
}