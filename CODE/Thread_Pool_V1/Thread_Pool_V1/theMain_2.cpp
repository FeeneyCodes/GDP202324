#include <iostream>
#include <string>
#include <vector>
// 
#include <Windows.h>	// All things windows
#define WIN32_LEAN_AND_MEAN	

#include "cGameEntity.h"

const float WORLD_SIZE = 1000.0f;

bool g_GameIsRunning = false;

// Note that there is no "critical section" class or object
// JUST a pointer to a critical section
//CRITICAL_SECTION g_AddNumbersTotal_CS;

// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float getRandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


struct sWorkerThreadInfo
{
	cGameEntity* pTheEntity = NULL;
	bool bRun = true;	// if false, thread is idle
	DWORD sleepTime;	// Wait time for sleeping
	bool bThreadAlive = true;	// If false, thread exits
};


DWORD WINAPI UpdateEntityThread(LPVOID lpParameter)
{
	// Get the specific pointer type
//	cGameEntity* pTheEntity = (cGameEntity*)lpParameter;
	sWorkerThreadInfo* pWTI = (sWorkerThreadInfo*)lpParameter;

	while (pWTI->bThreadAlive)
	{

		if (pWTI->bRun)
		{
			// CRIT
			pWTI->pTheEntity->Update();
			pWTI->pTheEntity->PrintPosition();
			// Updated, go to sleep
			pWTI->bRun = false;
			// CRIT
		}
		else
		{
			// Go to sleep (be idle) for a while
			Sleep(pWTI->sleepTime);
//			Sleep(threadWaitTime);
			pWTI->bRun = true;	// Wake up
		}
	}
	return 0;
}



void UpdateEntity(cGameEntity* pTheEntity)
{
	while (::g_GameIsRunning)
	{
//		Sleep(100);
		pTheEntity->Update();
		pTheEntity->PrintPosition();
	}
	return;
}




int main(int argc, char* argv[])
{

	const unsigned int NUM_THREADS = 64;
//	const unsigned int NUM_THREADS = 1'000;

	cGameEntity* pThePlayer = new cGameEntity();
	pThePlayer->position.x = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
	pThePlayer->position.y = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
	pThePlayer->position.z = 0.0f;


	std::vector<cGameEntity*> vec_pEntities;
	for (unsigned int count = 0; count != NUM_THREADS; count++)
	{
		cGameEntity* pNewEntity = new cGameEntity();

		pNewEntity->position.x = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
		pNewEntity->position.y = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
		pNewEntity->position.z = 0.0f;

		// Have them track the player. 
		// i.e. move towards the player 
		pNewEntity->SetTargetToChase(pThePlayer->position);
		pNewEntity->StartChaseTarget();

		vec_pEntities.push_back(pNewEntity);
	}


	std::cout << "Player: ";
	pThePlayer->PrintPosition();


	::g_GameIsRunning = true;
//	UpdateEntity(pEnemy);
//	void* pParams = (void*)pEnemy;


//	UpdateEntityThread(pParams);

	DWORD ThreadIds[NUM_THREADS] = { 0 };
	HANDLE threadHandles[NUM_THREADS] = { 0 };

	for (unsigned int index = 0; index != NUM_THREADS; index++)
	{
		sWorkerThreadInfo* pWTI = new sWorkerThreadInfo();
		pWTI->pTheEntity = vec_pEntities[index];
		pWTI->sleepTime = 1;
		pWTI->bRun = false;
		pWTI->bThreadAlive = true;
		void* pParams = (void*)pWTI;

		threadHandles[index] = CreateThread(
						NULL,					// lpThreadAttributes,
						0,						// dwStackSize,
						UpdateEntityThread,		// lpStartAddress,
						pParams,				//  lpParameter,
						0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
						&(ThreadIds[index]) ); // lpThreadId
	}


	std::cout << "From main thread." << std::endl;
	std::cout.flush();

//	pEnemy->PrintPosition();

	while (::g_GameIsRunning)
	{
//		pWTI->bRun = true;
//		Sleep(16);		// approx 60Hz

//		for (unsigned int index = 0; index != NUM_THREADS; index++)
//		{
//			// Set the entity we want the thread to update
//			// CRIT
//			pWTI->pTheEntity = vec_pEntities[index];
//			pWTI->bRun = true;
			// CRIT

//		int enemyIndex = rand() % NUM_THREADS;
//		vec_pEntities[enemyIndex]->PrintPosition();

		std::cout << "Main thread loop" << std::endl;
		std::cout << "Main thread loop" << std::endl;
		std::cout << "Main thread loop" << std::endl;
		std::cout << "Main thread loop" << std::endl;
		std::cout << "Main thread loop" << std::endl;
		Sleep(5);	// I've got something else to do


//		}

	}




//	InitializeCriticalSection(&::g_AddNumbersTotal_CS);






//	// Multiple thread example
//	DWORD ThreadIds[NUM_THREADS] = { 0 };
//	HANDLE threadHandles[NUM_THREADS] = { 0 };
//
//	for (unsigned int threadIndex = 0; threadIndex != NUM_THREADS; threadIndex++)
//	{
//
//
//		threadHandles[threadIndex] = CreateThread(
//			NULL,					// lpThreadAttributes,
//			0,						// dwStackSize,
//			UpdateEntity,		// lpStartAddress,
//			NULL, // pParams,//  lpParameter,
//			0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
//			&(ThreadIds[threadIndex])); // lpThreadId
//	}




	// 
//	DeleteCriticalSection(&::g_AddNumbersTotal_CS);

	return 0;
}






