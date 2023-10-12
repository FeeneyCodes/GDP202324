#pragma once

#include "cPlayer.h"
#include "iRobot.h"
//#include "cSuperRobot.h"
#include "cWeaponMaker.h"	// Factory object
#include <vector>

class cArena
{
public:
	~cArena();
	void Init(int numRobots, int numPlayers);


	void Update(double deltaTime);

	// Any static variable is available even if the object
	//	isn't created yet;
	static cArena* getTheArena(void);

	static int hi;

private:
	cArena();		// Can't be called by anything but "me" (i.e. the cArena class)

	std::vector< iRobot* > vecRobots;
	std::vector< cPlayer* > vecPlayers;

	cWeaponMaker* pTheWeaponFactory;

	// Static means "shared" because there's only one
	static cArena* m_pTheOneAndOnlyArena;

};
