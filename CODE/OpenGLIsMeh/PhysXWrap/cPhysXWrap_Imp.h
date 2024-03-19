#pragma once

#include <string>
#include <vector>

void exampleFunction(bool blah);

#include "cPhysicsObjectTypes.h"

class cPhysXWrap_Imp
{
public:
	cPhysXWrap_Imp();
	~cPhysXWrap_Imp();

	void initPhysics(bool interactive, std::string PVD_Host_Address);
	void cleanupPhysics(bool interactive);


	void update(void);

	void getSceneActors(std::vector<cPhysicsObjectTypes> &vecPhysActors);


	// HACK: Remove his later
	void HACK_ShootBall(void);
private:

};



