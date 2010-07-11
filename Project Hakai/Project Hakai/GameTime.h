#ifndef _GAMETIME_H
#define _GAMETIME_H

class GameTime
{
public:
	GameTime(float deltaTime, float totalTime)
	{
		m_fDeltaTime = deltaTime;
		m_fTotalTime = totalTime;
	}

	float GetDeltaTime() { return m_fDeltaTime; }
	float GetTotalTime() { return m_fTotalTime; }

private:
	float m_fDeltaTime;
	float m_fTotalTime;
};

#endif