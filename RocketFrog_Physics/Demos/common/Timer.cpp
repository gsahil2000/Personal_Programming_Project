#include "Timer.h"
#include <Windows.h>
#include <mmsystem.h>

// Hold internal timing data for the performance counter.
static bool qpcFlag;
static double qpcFrequency;

namespace RocketFrog
{
	Timer* RocketFrog::Timer::m_instance = nullptr;

	Timer::Timer()
	{}

	Timer* Timer::GetInstance()
	{
		return m_instance;
	}

	/// Internal time and clock functions
	unsigned int systemTime()
	{
		if (qpcFlag)
		{
			static LONGLONG qpcMillisPerTick;
			QueryPerformanceCounter((LARGE_INTEGER*)&qpcMillisPerTick);
			return unsigned(qpcMillisPerTick * qpcFrequency);
		}
		else
		{
			return timeGetTime();
		}
	}

	unsigned Timer::GetTime()
	{
		return systemTime();
	}

	unsigned long systemClock()
	{
		__asm {
			rdtsc;
		}
	}

	unsigned long Timer::GetClock()
	{
		return systemClock();
	}

	void initTime()
	{
		LONGLONG _time;

		qpcFlag = ( QueryPerformanceFrequency((LARGE_INTEGER*)&_time) > 0 );

		if (qpcFlag)
		{
			qpcFrequency = 1000.0 / _time;
		}
	}

	void Timer::Init()
	{
		initTime();

		if (m_instance == nullptr)
		{
			m_instance = new Timer();
		}

		m_instance->m_frameNumber = 0;

		m_instance->m_lastFrameTimeStamp = systemTime();
		m_instance->m_lastFrameDuration = 0;

		m_instance->m_lastFrameClockStamp = systemClock();
		m_instance->m_lastFrameClockTicks = 0;

		m_instance->m_isPaused = false;

		m_instance->m_averageFrameDuration = 0;
		m_instance->m_fps = 0;
	}

	void Timer::Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	void Timer::Update()
	{
		if (m_instance == nullptr)
			return;

		if (!m_instance->m_isPaused)
		{
			m_instance->m_frameNumber++;
		}

		/// update the timing information
		unsigned long _currentTime = systemTime();
		m_instance->m_lastFrameDuration = _currentTime - m_instance->m_lastFrameTimeStamp;
		m_instance->m_lastFrameTimeStamp = _currentTime;

		/// update the tick information
		unsigned long _currentClock = systemClock();
		m_instance->m_lastFrameClockTicks = _currentClock - m_instance->m_lastFrameClockStamp;
		m_instance->m_lastFrameClockStamp = _currentClock;

		if (m_instance->m_frameNumber > 1)
		{
			if (m_instance->m_averageFrameDuration <= 0)
			{
				m_instance->m_averageFrameDuration = (double)m_instance->m_lastFrameDuration;
			}
			else
			{
				m_instance->m_averageFrameDuration *= 0.99;
				m_instance->m_averageFrameDuration += 0.01 * (double)m_instance->m_lastFrameDuration;

				m_instance->m_fps = (float)(1000.0/m_instance->m_averageFrameDuration);
			}
		}
	}
}