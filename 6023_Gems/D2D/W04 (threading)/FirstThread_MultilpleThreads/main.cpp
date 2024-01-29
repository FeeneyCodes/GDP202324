#include "threads.h"

typedef struct _ARRAY_INIT_INFO
{
	int* pArray;
	int iNumElementsToInit;
	int iThreadNum;
} ARRAY_INIT_INFO;

DWORD WINAPI InitializeArray( PVOID pvParam )
{
	ARRAY_INIT_INFO* pInfo = (ARRAY_INIT_INFO*) pvParam;
	
	mycout << _TEXT( "Beginning array init on thread " )
		   << pInfo->iThreadNum << _TEXT( "\n" );

	for( int iLoop = 0; iLoop < pInfo->iNumElementsToInit; iLoop++ )
		pInfo->pArray[iLoop] = pInfo->iThreadNum;

	mycout <<  _TEXT( "Completed array initialization on thread " )
		   <<  pInfo->iThreadNum << _TEXT( "\n" );
	return 0;
}

#define NUM_THREADS 50
#define NUM_ELEMENTS_TO_INIT 10000

int main()
{
	std::cout << "MAXIMUM_WAIT_OBJECTS: " << MAXIMUM_WAIT_OBJECTS << std::endl;
	std::cout.flush();


	int* paiBigArray = new int[NUM_THREADS * NUM_ELEMENTS_TO_INIT];
	HANDLE ahThread[NUM_THREADS];
	ARRAY_INIT_INFO* paStrctArrayInfo = new ARRAY_INIT_INFO[NUM_THREADS];
	DWORD dw;

	for( int iLoop = 0; iLoop < NUM_THREADS; iLoop++)
	{
		paStrctArrayInfo[iLoop].iNumElementsToInit = NUM_ELEMENTS_TO_INIT;
		paStrctArrayInfo[iLoop].iThreadNum = iLoop;
		paStrctArrayInfo[iLoop].pArray = 
			&(paiBigArray[iLoop * NUM_ELEMENTS_TO_INIT]);

		// Note: here is an example of calling the "C run-time" version 
		// This is more platform neutral than the CreateThread(), 
		//	so you could use it on Linux, or whatever, but if you are 
		//	using "pure" Win32 code, you can't call it (potentially bypasses
		//	some of the things Windows is trying to do with the threads).
		//
		// However, note that it's very similar syntax...

		ahThread[iLoop] = chBEGINTHREADEX( NULL, 0, InitializeArray,
										   (PVOID)(&paStrctArrayInfo[iLoop]),
										   0, &dw );
	}

	// Only waits for 64 of them, not matter how many you call.
	WaitForMultipleObjects( NUM_THREADS, ahThread, TRUE, INFINITE );

	// Clean-up
	for ( int iLoop =  0; iLoop < NUM_THREADS; iLoop++)
		CloseHandle( ahThread[iLoop]);

	delete[] paiBigArray;
	delete[] paStrctArrayInfo;

	return 0;
}











