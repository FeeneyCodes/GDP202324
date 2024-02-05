#include <iostream>
#include <string>
#include <vector>
// 
#include <Windows.h>	// All things windows
#define WIN32_LEAN_AND_MEAN	



// Note that there is no "critical section" class or object
// JUST a pointer to a critical section
CRITICAL_SECTION g_AddNumbersTotal_CS;


unsigned long long g_NumberOfItems = 0;
unsigned long long g_NumItemsCritSection = 0;
LONG64 g_NumItemsAtomic = 0;
// Same as long long (signed 64 bit integer)


// Each thread will get a vector of numbers, 
//	but it will add its total to the GrandTotal
DWORD WINAPI AddThemUpThreaded(LPVOID lpParameter)
{
	unsigned long long randChange = rand() % 100;

	Sleep(250);

	for ( unsigned int count = 0; count != 100; count++ )
	{
		// unprotected value
		::g_NumberOfItems += randChange;
//		::g_NumberOfItems = ::g_NumberOfItems + randChange;

		EnterCriticalSection(&::g_AddNumbersTotal_CS);
		::g_NumItemsCritSection += randChange;
//		::g_NumItemsCritSection = ::g_NumItemsCritSection + randChange;
		LeaveCriticalSection(&::g_AddNumbersTotal_CS);

		// Atomic:
		// https://learn.microsoft.com/en-us/windows/win32/api/winnt/nf-winnt-interlockedexchangeadd64
		InterlockedExchangeAdd64(&::g_NumItemsAtomic, randChange);
	}


	return 0;
}


bool g_IsLocked = false;
int g_counter = 0;

unsigned long long g_atomicNumber = 6;
unsigned long long g_atomicNumberInterlocked = 6;


int main(int argc, char* argv[])
{

	InitializeCriticalSection(&::g_AddNumbersTotal_CS);



	EnterCriticalSection(&g_AddNumbersTotal_CS);
	// Flip the flag value
	::g_IsLocked = !::g_IsLocked;
	// Do something...
	LeaveCriticalSection(&g_AddNumbersTotal_CS);

	// Increment the counter
	::g_counter++;

	std::cout << "Hey" << std::endl;

	::g_counter += 5;

	std::cout << "Hey!" << std::endl;

	::g_counter = ::g_counter + 7;



	std::cout << "Non Atomic" << std::endl;
	unsigned long long theCopy = ::g_atomicNumber;
	theCopy += 81;
	::g_atomicNumber = theCopy;

	std::cout << ::g_atomicNumber << std::endl;

	std::cout << "Non Atomic" << std::endl;

	// In one instruction, it will place 99 into the location
	// AND get the previous value in one "atomic" (protected) step
	unsigned long long theCopy2 = InterlockedExchange(&g_atomicNumberInterlocked, 99);

	std::cout << "Atomic" << std::endl;




	const unsigned int NUM_THREADS = 100;

//	InitializeCriticalSection(&::g_AddNumbersTotal_CS);


	// Multiple thread example
	DWORD ThreadIds[NUM_THREADS] = { 0 };
	HANDLE threadHandles[NUM_THREADS] = { 0 };

	for (unsigned int threadIndex = 0; threadIndex != NUM_THREADS; threadIndex++)
	{
//		sAddUpInfo* pNumInfo = &(numberInfoForThread[threadIndex]);
//		void* pParams = (void*)pNumInfo;

//		void* pParams = (void*)&(numberInfoForThread[threadIndex]);

		threadHandles[threadIndex] = CreateThread(
			NULL,					// lpThreadAttributes,
			0,						// dwStackSize,
			AddThemUpThreaded,		// lpStartAddress,
			NULL, // pParams,//  lpParameter,
			0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
			&(ThreadIds[threadIndex])); // lpThreadId
	}


	Sleep(1000);

	std::cout << "g_NumberOfItems = " << g_NumberOfItems << std::endl;
	std::cout << "g_NumItemsCritSection = " << g_NumItemsCritSection << std::endl;

	// Because this is volatile, we need to use the instruction again
	//	to get the value
	long long valOf_g_NumItemsAtomic = InterlockedExchangeAdd64(&::g_NumItemsAtomic, 0);

	std::cout << "g_NumItemsAtomic = " << valOf_g_NumItemsAtomic << std::endl;


	// 
	DeleteCriticalSection(&::g_AddNumbersTotal_CS);

	return 0;
}






