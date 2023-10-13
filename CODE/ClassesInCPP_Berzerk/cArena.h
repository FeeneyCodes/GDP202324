#pragma once

#include "cPlayer.h"
#include "iRobot.h"
//#include "cSuperRobot.h"
#include "cWeaponMaker.h"	// Factory object
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include "iRobotQuery.h"

#include "iMediator.h"

class cArena :
	public iRobotQuery,
	public iMediator
{
public:
	~cArena();
	// Assume arena is square. 
	void Init(int numRobots, int numPlayers, 
			  float arenaSizeInMeters = 250.0f);


	void Update(double deltaTime);

	// Any static variable is available even if the object
	//	isn't created yet;
	static cArena* getTheArena(void);

	// from the iRobotQuery interface
	virtual glm::vec2 findClosestRobot(glm::vec2 whereIAm);


	// iGetRobotLocations()
//	std::vector< iRobot* > getTheVectorOfRobotsYo(void)
//	{
//		return vecRobots;
//	}

//	static int hi;

private:
	cArena();		// Can't be called by anything but "me" (i.e. the cArena class)

	std::vector< iRobot* > vecRobots;
	std::vector< cPlayer* > vecPlayers;

	cWeaponMaker* pTheWeaponFactory;

	// Static means "shared" because there's only one
	static cArena* m_pTheOneAndOnlyArena;

	float m_arenaSizeInMeters;

};
