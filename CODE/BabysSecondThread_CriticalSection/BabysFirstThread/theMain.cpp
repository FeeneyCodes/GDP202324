#include <iostream>
#include <string>
#include <vector>
// 
#include <Windows.h>	// All things windows
#define WIN32_LEAN_AND_MEAN	

//float AddThemUp( std::vector<float> &vecNums )
//{
//	float theSum = 0;
//	for ( float curNum : vecNums )
//	{
//		theSum += curNum;
//	}
//	return theSum;
//}

// Note that there is no "critical section" class or object
// JUST a pointer to a critical section
CRITICAL_SECTION g_AddNumbersTotal_CS;

struct sAddUpInfo
{
	std::vector<unsigned long long> vecTheNumbers;
	unsigned long long total = 0;
	bool bTheadIsDone = false;
	// 
	unsigned long long* pGrandTotal = NULL;
};

// Each thread will get a vector of numbers, 
//	but it will add its total to the GrandTotal
DWORD WINAPI AddThemUpThreaded(LPVOID lpParameter)
{
//	sAddUpInfo* pThreadInfo = &numberInfoForThread;
//	void* pParams = (void*)pThreadInfo;

	// Cast this back to the type we want (original value)
	sAddUpInfo* pThreadInfo = (sAddUpInfo*)lpParameter;

	Sleep(100);

	for (unsigned long long curNum : pThreadInfo->vecTheNumbers)
	{
		pThreadInfo->total += curNum;
	}

	Sleep(100);

	EnterCriticalSection( &::g_AddNumbersTotal_CS);

	// Update the grand total from our total
	unsigned long long grandTotal = *(pThreadInfo->pGrandTotal);
	grandTotal += pThreadInfo->total;
//	// Save it
	*(pThreadInfo->pGrandTotal) = grandTotal;

//	(*(pThreadInfo->pGrandTotal)) += pThreadInfo->total;

	LeaveCriticalSection(&::g_AddNumbersTotal_CS);

	pThreadInfo->bTheadIsDone = true;

	return 0;
}


//if (TryEnterCriticalSection(&::g_AddNumbersTotal_CS) == TRUE)
//{
//	// Update the grand total from our total
//	unsigned long long grandTotal = *(pThreadInfo->pGrandTotal);
//	grandTotal += pThreadInfo->total;
////	// Save it
//	*(pThreadInfo->pGrandTotal) = grandTotal;
//
////	(*(pThreadInfo->pGrandTotal)) += pThreadInfo->total;
//
//	LeaveCriticalSection(&::g_AddNumbersTotal_CS);
//}

int main(int argc, char* argv[])
{

//	const unsigned int NUM_THREADS = 64;
	const unsigned int NUM_THREADS = 1'000;

	sAddUpInfo numberInfoForThread[NUM_THREADS];

	unsigned long long theGrandTotal = 0;

	for ( unsigned int index = 0; index != NUM_THREADS; index++ )
	{
		numberInfoForThread[index].pGrandTotal = &theGrandTotal;

		int count = 0;
		int numNumbers = (rand() % 50'000) + 10'000;
		while ( count < numNumbers)
		{
			numberInfoForThread[index].vecTheNumbers.push_back((unsigned long long)(rand() % 1'000));
			count++;
		}
	}

	InitializeCriticalSection(&::g_AddNumbersTotal_CS);


	// Multiple thread example
	DWORD ThreadIds[NUM_THREADS] = {0};
	HANDLE threadHandles[NUM_THREADS] = { 0 };

	for ( unsigned int threadIndex = 0; threadIndex != NUM_THREADS; threadIndex++ )
	{
//		sAddUpInfo* pNumInfo = &(numberInfoForThread[threadIndex]);
//		void* pParams = (void*)pNumInfo;

		void* pParams = (void*) &(numberInfoForThread[threadIndex]);

		threadHandles[threadIndex] = CreateThread(
			NULL,					// lpThreadAttributes,
			0,						// dwStackSize,
			AddThemUpThreaded,		// lpStartAddress,
			pParams,//  lpParameter,
			0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
			&(ThreadIds[threadIndex]) ); // lpThreadId
	}

//	DWORD WaitForMultipleObjects(
//		[in] DWORD        nCount,
//		[in] const HANDLE* lpHandles,
//		[in] BOOL         bWaitAll,
//		[in] DWORD        dwMilliseconds
//	);

	// Note that you can pass ANY number of threads, but
	//	it'll only wait for MAXIMUM_WAIT_OBJECTS
	// Which is 64 on VS2022
	// (usually 32 or 64 or something small)
	// #define MAXIMUM_WAIT_OBJECTS 64     // Maximum number of wait objects

//	WaitForMultipleObjects(NUM_THREADS,
//						   threadHandles,
//						   TRUE,
//						   INFINITE);

	// There are 1,000 threads so we'll look at that boolean
	bool bKeepWaiting = false;
	do 
	{
		// Assume we are done...
		bKeepWaiting = false;
		for (unsigned int index = 0; index != NUM_THREADS; index++)
		{
			// If any thread ISN'T done, set flag
			if ( numberInfoForThread[index].bTheadIsDone == false )
			{
				bKeepWaiting = true;

			}
		}
	} while (bKeepWaiting);

	// Add up the individual one 
	unsigned long long actualGrandTotal = 0;
	for (unsigned int index = 0; index != NUM_THREADS; index++)
	{
		std::cout << "Thread[" << index << "] sum: " << numberInfoForThread[index].total << std::endl;
		actualGrandTotal += numberInfoForThread[index].total;
	}
	std::cout << "Actual: " << actualGrandTotal << std::endl;

	// Print the grand total
	std::cout << "Thread sum: " << theGrandTotal << std::endl;


	// 
	DeleteCriticalSection(&::g_AddNumbersTotal_CS);

	return 0;
}






