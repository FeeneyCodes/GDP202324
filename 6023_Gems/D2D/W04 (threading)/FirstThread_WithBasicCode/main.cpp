#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <process.h>

#include <string>

class cNinja
{
public:
	static DWORD WINAPI OurFirstThread( PVOID pvParam );
	std::string name;
};

// Note that this is POINTER to a critical section
CRITICAL_SECTION CS_KillCount;
unsigned int g_killCount = 0;


// NOT ON THE CP
//glm::vec4 wheresTheDoctorAt;


void AddNumberToCount(int theNumber)
{
	// This variable is shared by more than one thread
	EnterCriticalSection( &CS_KillCount );

	// Be in and out as fast as possible
	g_killCount += theNumber;

	LeaveCriticalSection( &CS_KillCount );
	return;
}

DWORD WINAPI OurFirstThread( PVOID pvParam )
{
	
	//int* pShoeSize = (int*)pvParam; //(is the pointer to void, but it's really pointing to an int)
	//int ShoeSize = *pShoeSize;
	//std::cout << "ShoeSize = " << ShoeSize << std::endl;
	//std::cout.flush();

	cNinja* pNinja = (cNinja*)pvParam;		// From void* to cNinja*
//	std::cout << pNinja->name << std::endl;
//	std::cout.flush();

	std::cout << "Hey, I'm a ninja " 
		<< pNinja->name 
		<< "! Fear me." << std::endl;

	int limit = rand() % 25;
	std::cout << pNinja->name << "Making " << limit << " kills" << std::endl;

	for ( int count = 0; count < limit; count++ )
	{
		std::cout << pNinja->name << " Adding 1 kill!" << std::endl;

	//	count++;

		AddNumberToCount(1);

		std::cout << "count = " << count << std::endl;
		std::cout.flush();
		Sleep(250);			// in ms
	}

	// Go to sleep, go to sleep, you are driving me crazy...
//	MessageBox(NULL, "HEY","Waiting (thread 1)", MB_OK);

	// HAS to return (just like ANSI main C program)
	return 0;
}

DWORD WINAPI OurSecondThread( PVOID pvParam )
{
	// This is the thread's code, like an application's
	//	code is in the main() function.
	// We can have any number of these.
	// When this function returns, the thread is done.

	// Go to sleep, go to sleep, you are driving me crazy...
	MessageBox(NULL, "HEY","Waiting on thread 2", MB_OK);
	Sleep(2000);			// in ms

	// HAS to return (just like ANSI main C program)
	return 0;
}

int main( int argc, char* argv[] )
{
	srand(1000);
	//LPDWORD phThread1;	// WIll POINT to the variable 
	//LPDWORD phThread2;	// WIll POINT to the variable 
	////					//	holding the thread id
	//DWORD hThreadID1;	// Will HAVE the ID in it
	//DWORD hThreadID2;	// Will HAVE the ID in it
	//phThread1 = &hThreadID1;	// Set pointer to point to variable
	//phThread2 = &hThreadID2;	// Set pointer to point to variable

	LPDWORD phThreads[2] = {0};	// Clear to zero
	DWORD hThreads[2] = {0};	
	HANDLE hThreadHandles[2] = {0};

	phThreads[0] = &(hThreads[0]);
	phThreads[1] = &(hThreads[1]);

	// Init the critical section
	InitializeCriticalSection( &CS_KillCount );


	//int ShoeSize = 11;
	//int* pShoeSize = &ShoeSize;		// Get pointer to shoe size
	//void* pVShoeSize = (void*)pShoeSize;

	cNinja* pMichael = new cNinja();
	pMichael->name = "Michael";
	void* pV_Michael = (void*)pMichael;

	cNinja* pRobin = new cNinja();
	pRobin->name = "Robin";
	void* pV_Robin = (void*)pRobin;

	// Check for failure... will return NULL
//	HANDLE hThreadHandle1;
	hThreadHandles[0] = CreateThread(NULL,	// Default security
								 0,		// Stack size - default - win32 = 1 Mbyte
								 &OurFirstThread, // Pointer to the thread's function
								 pMichael,		// The value (parameter) we pass to the thread
										// This is a pointer to void... more on this evil thing later...
								 0,  // CREATE_SUSPENDED or 0 for threads...
								 (DWORD*) &phThreads[0]);		// pointer or ref to variable that will get loaded with threadID
	hThreadHandles[1] = CreateThread(NULL,	// Default security
								 0,		// Stack size - default - win32 = 1 Mbyte
								 &OurFirstThread, // Pointer to the thread's function
								 pRobin,		// The value (parameter) we pass to the thread
										// This is a pointer to void... more on this evil thing later...
								 0,  // CREATE_SUSPENDED or 0 for threads...
								 (DWORD*) &phThreads[1]);		// pointer or ref to variable that will get loaded with threadID
	//HANDLE hThreadHandle2;
	//hThreadHandle2 = CreateThread(NULL,	// Default security
	//							 0,		// Stack size - default - win32 = 1 Mbyte
	//							 &OurSecondThread, // Pointer to the thread's function
	//							 0,		// The value (parameter) we pass to the thread
	//									// This is a pointer to void... more on this evil thing later...
	//							 0,  // CREATE_SUSPENDED or 0 for threads...
	//							 (DWORD*) &phThread2);		// pointer or ref to variable that will get loaded with threadID

		// This last line could also be:
		//		&hThread);
		//		phNewThread);

	// Now that the thread is started... wait for it...
	std::cout << "Waiting for the thread to exit..." << std::endl;
	std::cout.flush();
//	if (WaitForSingleObject((hThreadHandle1), INFINITE))
	if (WaitForMultipleObjects(2, hThreadHandles, TRUE, INFINITE ))
	{
		std::cout << "Closing the thread...";
		std::cout.flush();
//		CloseHandle(hThreadHandle1);
	}
	//if (WaitForSingleObject((hThreadHandle2), INFINITE))
	//{
	//	std::cout << "Closing the thread...";
	//	std::cout.flush();
	//	CloseHandle(hThreadHandle2);
	//}
	std::cout << "closed" << std::endl;


	// Bye bye critical section
	DeleteCriticalSection( &CS_KillCount );


	return 0;
}

	 
bool bIsBufferLocked(void);
void LockBuffer(void);
void UnlockBuffer(void);


CRITICAL_SECTION CS_BufferLock;
bool g_bIsBufferLocked = false;


void Blah()
{
	if ( ! bIsBufferLocked() ) 
	{
		LockBuffer();
		// Do something with the buffer

		UnlockBuffer();
	}
}

bool bIsBufferLocked(void)
{
	return g_bIsBufferLocked;
}

void LockBuffer(void)
{
	EnterCriticalSection(&CS_BufferLock);
	g_bIsBufferLocked = true;
	LeaveCriticalSection(&CS_BufferLock);
	return;
}

void UnlockBuffer(void)
{
	EnterCriticalSection(&CS_BufferLock);
	g_bIsBufferLocked = false;
	LeaveCriticalSection(&CS_BufferLock);
	return;
}



