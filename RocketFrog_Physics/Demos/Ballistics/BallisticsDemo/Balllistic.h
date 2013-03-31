#pragma once
#include <Particle.h>
#include <Application.h>
#include <Timer.h>

namespace RocketFrog
{
	class BallisticDemo : public Application
	{
	public:
		BallisticDemo();
		~BallisticDemo();

		virtual const char* GetTitle();

		virtual void Update();

		virtual void Display();

		virtual void Mouse(const int a_key, const int a_state, const int x, const int y);

		virtual void Key(const unsigned char a_key);
	private:
		enum ShotType
		{
			NONE = 0,
			PISTOL,
			ARTILLERY,
			FIREBALL,
			LASER,
			END
		};

		/// Holds single ammunition round record
		struct AmmoRound
		{
			Particle m_particle;
			ShotType m_shotType;
			unsigned m_startTime;

			/// Draws the round
			void Render();
		};

		/// maximum number of rounds that can be fired.
		const static unsigned m_maxAmmo = 16;

		/// holds the particle data.
		AmmoRound m_ammo[m_maxAmmo];

		/// holds current shot type.
		ShotType m_currentShotType;

		/// fires a round.
		void Fire();
	};

	Application* GetApplication();
}