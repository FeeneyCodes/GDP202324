#pragma once

#include "cSoftBodyVerlet.h"


struct sSoftBodyThreadInfo
{
    cSoftBodyVerlet* p_theSoftBody;
    // This is the ideal framerate we want
    double desiredUpdateTime = 0.0;
    // Stuff to control the thread (which never exits)
    bool bRun = false;	// if false, thread is idle
    bool bIsAlive = true;
    DWORD sleepTime;	// Wait time for sleeping
};