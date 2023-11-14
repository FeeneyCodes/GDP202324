#pragma once

#include <vector>

class cHiResTimer
{
public:
//	cHiResTimer();
	cHiResTimer(unsigned int numberOfSamples);
	double getFrameTime_CRAPPY(void);
	// Calculate the average frame time
	double getFrameTime(void);

	void setMaxDeltaTime(double newMaxDeltaTime);
private:
	// Can't call this
	cHiResTimer();

	double m_lastTime;

	std::vector<double> m_vecDeltaTimes;

	unsigned int m_nextBufferIndex = 0;

	// 30 FPS
	static const double MAX_DELTA_TIME_DEFAULT;// = 1.0 / 30.0;

	double m_MaxDeltaTime;
};

