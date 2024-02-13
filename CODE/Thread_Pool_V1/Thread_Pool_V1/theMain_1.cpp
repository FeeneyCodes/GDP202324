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

void Dothis(void)
{

	return;
}

class iThreadUpdate
{
public:
	virtual ~iThreadUpdate() {};

	virtual void Update(double deltaTime) = 0;
	static void DoThis(void);
	void* pFunctionToRun;
};

struct sWorkerThreadInfo
{
	cGameEntity* pTheEntity = NULL;
	bool bRun = true;	// if false, thread is idle
	DWORD sleepTime;	// Wait time for sleeping
	bool bThreadAlive = true;	// If false, thread exits
	iThreadUpdate* pThreadJob = NULL;
};


DWORD WINAPI UpdateEntityThread(LPVOID lpParameter)
{
	void* pFunction = Dothis;
	void* pFunction = iThreadUpdate::DoThis;
//	void* pFunction = iThreadUpdate::Update;


	// Get the specific pointer type
//	cGameEntity* pTheEntity = (cGameEntity*)lpParameter;
	sWorkerThreadInfo* pWTI = (sWorkerThreadInfo*)lpParameter;

//	DWORD threadWaitTime = 0.0;
//	DWORD idealWaitTime = pWTI->sleepTime;

//	double lastTime = glfwGetTime();

	while (pWTI->bThreadAlive)
	{
//		double currentTime = glfwGetTime();
//		double deltaTime = currentTime - lastTime;
//		lastTime = currentTime;
//
//		if ( deltaTime < idealWaitTime )
//		{
//			threadWaitTime++;
//		}
//		if ( deltaTime > idealWaitTime )
//		{
//			threadWaitTime--;
//		}


		if ( pWTI->bRun )
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
//			Sleep(0);
		}
	}
	return 0;
}



void UpdateEntity(cGameEntity* pTheEntity)
{
	while ( ::g_GameIsRunning )
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

	std::vector<cGameEntity*> vec_pEntities;
	for ( unsigned int count = 0; count != NUM_THREADS; count++ )
	{
		cGameEntity* pNewEntity = new cGameEntity();

		pNewEntity->position.x = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
		pNewEntity->position.y = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
		pNewEntity->position.z = 0.0f;

		vec_pEntities.push_back(pNewEntity);
	}

	cGameEntity* pThePlayer = new cGameEntity();
	pThePlayer->position.x = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
	pThePlayer->position.y = getRandomFloat(-WORLD_SIZE, WORLD_SIZE);
	pThePlayer->position.z = 0.0f;


	// Have ONE of these dudes track the player. 
	// i.e. move towards the player 

	cGameEntity* pEnemy = vec_pEntities[0];
	pEnemy->SetTargetToChase(pThePlayer->position);
	pEnemy->StartChaseTarget();

	std::cout << "Player: ";
	pThePlayer->PrintPosition();


	::g_GameIsRunning = true;
//	UpdateEntity(pEnemy);
//	void* pParams = (void*)pEnemy;


	sWorkerThreadInfo* pWTI = new sWorkerThreadInfo();
	pWTI->pTheEntity = pEnemy;
	pWTI->sleepTime = 1;
//	pWTI->sleepTime = 0;
	pWTI->bRun = false;
	pWTI->bThreadAlive = true;
	void* pParams = (void*)pWTI;


//	UpdateEntityThread(pParams);

	DWORD ThreadId = 0;
	HANDLE threadHandle = 0;
	threadHandle = CreateThread(
		NULL,					// lpThreadAttributes,
		0,						// dwStackSize,
		UpdateEntityThread,		// lpStartAddress,
		pParams,				//  lpParameter,
		0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
		&ThreadId); // lpThreadId


	std::cout << "From main thread." << std::endl;
	std::cout.flush();

//	pEnemy->PrintPosition();

	while (::g_GameIsRunning)
	{
//		pWTI->bRun = true;
//		Sleep(16);		// approx 60Hz

		for ( unsigned int index = 0; index != NUM_THREADS; index++ )
		{
			// Set the entity we want the thread to update
			// CRIT
			pWTI->pTheEntity = vec_pEntities[index];
			pWTI->bRun = true;
			// CRIT
			Sleep(5);	// I've got something else to do
		}

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






