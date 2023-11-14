#include "cHiResTimer.h"

#include "OpenGLCommon.h"


//cHiResTimer::cHiResTimer()
//{
//    this->m_lastTime = glfwGetTime();
//}

cHiResTimer::cHiResTimer(unsigned int numberOfSamples)
{
    this->m_lastTime = glfwGetTime();

    this->m_MaxDeltaTime = cHiResTimer::MAX_DELTA_TIME_DEFAULT;
    
    for ( int count = 0; count != numberOfSamples; count++ )
    {
        this->m_vecDeltaTimes.push_back(cHiResTimer::MAX_DELTA_TIME_DEFAULT);
    }
}

//static 
const double cHiResTimer::MAX_DELTA_TIME_DEFAULT = 1.0 / 30.0;

void cHiResTimer::setMaxDeltaTime(double newMaxDeltaTime)
{
    this->m_MaxDeltaTime = newMaxDeltaTime;
    return;
}

double cHiResTimer::getFrameTime(void)
{
    // Get the delta since last time we checked
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - m_lastTime;

    if (deltaTime > cHiResTimer::MAX_DELTA_TIME_DEFAULT)
    {
        deltaTime = cHiResTimer::MAX_DELTA_TIME_DEFAULT;
    }

//    Sleep(0);

    this->m_lastTime = currentTime;

    this->m_vecDeltaTimes[this->m_nextBufferIndex] = deltaTime;
    this->m_nextBufferIndex++;

    // Is the index out of bounds?
    if (this->m_nextBufferIndex >= (unsigned int)this->m_vecDeltaTimes.size())
    {
        this->m_nextBufferIndex = 0;
    }

    // You can also mask the number by the max number IF it's a multiple of 2
    //  1 -> 0000 0001
    //  2 -> 0000 0010 
    //  4 -> 0000 0100
    //  8 -> 0000 1000
    // 16 -> 0001 0000
    // 31 -> 0001 1111
    // 32 -> 0010 0000
//    this->m_nextBufferIndex = this->m_nextBufferIndex & 31;

    
    // Get the average
    double totalDeltaTime = 0.0;
    for ( unsigned int index = 0; index != this->m_vecDeltaTimes.size(); index++ )
    {
        totalDeltaTime += this->m_vecDeltaTimes[index];
    }

    double averageDelta = totalDeltaTime / (double)this->m_vecDeltaTimes.size();

    return averageDelta;
}






double cHiResTimer::getFrameTime_CRAPPY(void)
{
        // Time per frame (more or less)
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - m_lastTime;
    // We should add this to a list of times, and get the average frame time
    const double LARGEST_DELTA_TIME = 1.0f / 30.0f; // 30FPS  (16 ms)  24 FPS
    if (deltaTime > LARGEST_DELTA_TIME)
    {
        deltaTime = LARGEST_DELTA_TIME;
    }
    m_lastTime = currentTime;

    return deltaTime;
}


