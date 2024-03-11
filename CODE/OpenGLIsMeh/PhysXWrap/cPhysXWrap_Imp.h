#pragma once

#include <string>


void exampleFunction(bool blah);

class cPhysXWrap_Imp
{
public:
	cPhysXWrap_Imp();
	~cPhysXWrap_Imp();

	void initPhysics(bool interactive, std::string PVD_Host_Address);
	void cleanupPhysics(bool interactive);

	void update(void);

private:

};



