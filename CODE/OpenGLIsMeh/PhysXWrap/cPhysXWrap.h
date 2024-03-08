#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Forward declare for pointer to implementation
class cPhysXWrap_Imp;

#include <string>

class cPhysXWrap
{
public:
	cPhysXWrap();
	~cPhysXWrap();

	void initPhysics(bool interactive, std::string PVD_Host_Address = "127.0.0.1");
	void cleanupPhysics(bool interactive);

private:
	cPhysXWrap_Imp* m_pImp = nullptr;
};

