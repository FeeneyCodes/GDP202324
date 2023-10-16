#pragma once

#include "cArena.h"

#pragma once

#include "cPlayer.h"
#include "iRobot.h"
//#include "cSuperRobot.h"
#include "cWeaponMaker.h"	// Factory object
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>


class cArena_Implementation : 
	public iRobotQuery,
	public iMediator
{
public:
	cArena_Implementation();		// Can't be called by anything but "me" (i.e. the cArena class)
	~cArena_Implementation();

	// Assume arena is square. 
	void Init(int numRobots, int numPlayers,
			  float arenaSizeInMeters);


	void Update(double deltaTime);


	// from the iRobotQuery interface
	virtual glm::vec2 findClosestRobot(glm::vec2 whereIAm);


	// From the iMediator interface
	virtual sMessage getMessage(sMessage theMessage);
	virtual void recieveMessage(sMessage theMessage);



private:

	std::vector< iRobot* > vecRobots;
	std::vector< cPlayer* > vecPlayers;

	cWeaponMaker* pTheWeaponFactory;

	float m_arenaSizeInMeters;

	unsigned int numberOfRobots;

	float timeLeftInGame;

};
