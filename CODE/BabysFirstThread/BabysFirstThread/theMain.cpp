#include <iostream>
#include <string>
#include <vector>
// 
#include <Windows.h>	// All things windows
#define WIN32_LEAN_AND_MEAN	

float AddThemUp( std::vector<float> &vecNums )
{
	float theSum = 0;
	for ( float curNum : vecNums )
	{
		theSum += curNum;
	}
	return theSum;
}

std::vector<float> g_vecNums;
float g_theTotal = 0.0f;



struct sAddUpInfo
{
	std::vector<float> vecTheNumbers;
	float total = 0.0f;
	bool bTheadIsDone = false;
};


DWORD WINAPI AddThemUpThreaded(LPVOID lpParameter)
{
//	sAddUpInfo* pThreadInfo = &numberInfoForThread;
//	void* pParams = (void*)pThreadInfo;

	// Cast this back to the type we want (original value)
	sAddUpInfo* pThreadInfo = (sAddUpInfo*)lpParameter;

//	std::string* pOHNO = (std::string*)lpParameter;
//	pOHNO->length();

//	float theSum = 0;
	for (float curNum : pThreadInfo->vecTheNumbers)
	{
//		std::cout << "current number: " << curNum << std::endl;
		pThreadInfo->total += curNum;
	}

// sun is "returned" via the passed parameter

	pThreadInfo->bTheadIsDone = true;

	// The OS can use this, sort of like the return value from the main() function.
	// I'd suggest just ignoring this.
	return 0;
}

int main(int argc, char* argv[])
{
//	std::vector<float> vecTheNumbers;

	sAddUpInfo numberInfoForThread;

	int count = 0;
	while ( count < 100'000 )
	{
//		vecTheNumbers.push_back( (float)(rand() % 1'000) );
//		::g_vecNums.push_back( (float)(rand() % 1'000) );
		numberInfoForThread.vecTheNumbers.push_back((float)(rand() % 1'000));
		count++;
	}


//	unsigned int theTotal = AddThemUp(vecTheNumbers);

	// LP stands for "long pointer" (referring to the size of the pointer or the near/far call"
	DWORD ThreadId = 0;

	// Getting the pointer to void to give to the thread..
	sAddUpInfo* pThreadInfo = &numberInfoForThread;
	void* pParams = (void*)pThreadInfo;

//	void* pParams = (void*)(&numberInfoForThread);

	HANDLE threadHandle = CreateThread(
		NULL,					// lpThreadAttributes,
		0,						// dwStackSize,
		AddThemUpThreaded,		// lpStartAddress,
		pParams,//  lpParameter,
		0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
		&ThreadId); // lpThreadId

//	HANDLE threadHandle = CreateThread(
//		NULL,					// lpThreadAttributes,
//		0,						// dwStackSize,
//		AddThemUpThreaded_globalVariables,		// lpStartAddress,
//		0,//  lpParameter,
//		0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
//		&ThreadId); // lpThreadId
	// Move on instantly to other Very Important things.
	// I'm an important thread with stuff to do, you know...
	
//	HANDLE CreateThread(
//		[in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
//		[in]            SIZE_T                  dwStackSize,
//		[in]            LPTHREAD_START_ROUTINE  lpStartAddress,
//		[in, optional]  __drv_aliasesMem LPVOID lpParameter,
//		[in]            DWORD                   dwCreationFlags,
//		[out, optional] LPDWORD                 lpThreadId
//	);
//

//	unsigned int timeWasterCount = 0; 
//	while ( timeWasterCount < 1'000'000 )
//	{
//		timeWasterCount++;
//	}


//	std::cout << "going to sleep..." << std::endl;
//	Sleep(1000);
//	std::cout << "Waking up" << std::endl;

	// Wait for thread to finish
//	DWORD WaitForSingleObject(
//		[in] HANDLE hHandle,
//		[in] DWORD  dwMilliseconds
//	);


//	std::cout << "Waiting for thread to finish..." << std::endl;
//	WaitForSingleObject(threadHandle, INFINITE);
//	std::cout << "...eets dun." << std::endl;

	while ( ! pThreadInfo->bTheadIsDone )
	{
		std::cout << "Still waiting..." << std::endl;
	}


	// You could also see if the thread is done with 
	//	GetExitCodeThread() and if not, continue on and check later
	// Why use this? (with 0 ms)
	// To see if the thread is done its stuff.
	// (otherwise continue on)
//	WaitForSingleObject(threadHandle, 0);

//	std::cout << ::g_theTotal << std::endl;
//	std::cout << theTotal << std::endl;
	std::cout << numberInfoForThread.total << std::endl;

	return 0;
}
















// Thread function has to be in this format (signature)
//https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms686736(v=vs.85)
// LPVOID is "void*"
DWORD WINAPI AddThemUpThreaded_globalVariables(LPVOID lpParameter)
{
	float theSum = 0;
	for (float curNum : ::g_vecNums)
	{
//		std::cout << "current number: " << curNum << std::endl;
		theSum += curNum;
	}
//	return theSum;
	::g_theTotal = theSum;

	// The OS can use this, sort of like the return value from the main() function.
	// I'd suggest just ignoring this.
	return 0;
}