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

// Thread function has to be in this format (signature)
//https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms686736(v=vs.85)
// LPVOID is "void*"
DWORD WINAPI AddThemUpThreaded( LPVOID lpParameter )
{
	float theSum = 0;
	for ( float curNum : ::g_vecNums)
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



int main(int argc, char* argv[])
{
//	std::vector<float> vecTheNumbers;
	int count = 0;
	while ( count < 10 )
	{
//		vecTheNumbers.push_back( (float)(rand() % 1'000) );
		::g_vecNums.push_back( (float)(rand() % 1'000) );
		count++;
	}


//	unsigned int theTotal = AddThemUp(vecTheNumbers);

	// LP stands for "long pointer" (referring to the size of the pointer or the near/far call"
	DWORD ThreadId = 0;

	HANDLE threadHandle = CreateThread(
		NULL,					// lpThreadAttributes,
		0,						// dwStackSize,
		AddThemUpThreaded,		// lpStartAddress,
		0,//  lpParameter,
		0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
		&ThreadId); // lpThreadId

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

	std::cout << "Waiting for thread to finish..." << std::endl;
	WaitForSingleObject(threadHandle, INFINITE);
	std::cout << "...eets dun." << std::endl;

	std::cout << ::g_theTotal << std::endl;

//	std::cout << theTotal << std::endl;

	return 0;
}