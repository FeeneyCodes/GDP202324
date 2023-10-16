#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

//#include "cRayGun.h"
#include "iWeapon.h"
#include "iRobot.h"

#include "iRobotPlacement.h"

// The arena
//#include "iRobotQuery.h"
//#include "iAssignRobotQuery.h"

#include <vector>


class cRobot :
	public iRobot
//	public iRobotPlacement,
//	public iAssignRobotQuery
{
public:
	cRobot();		// constructor (c'tor)
	~cRobot();		// destructor (d'tor)

	glm::vec2 pos;	// xy position
	bool bIsDead;
	glm::vec2 dir;	// direction
	bool bNoticedPlayer;

	iWeapon* pTheWeapon;

	// From iRobot interface
	virtual void Attack(void);
//	virtual void Move(glm::vec2 toWhere);
	virtual void Update(double deltaTime);


	// From iRobotPlacement interface
	virtual void setInitalLocation(glm::vec2 startingPosition);
	virtual glm::vec2 getLocation(void);

	virtual void set_RobotQuery(iMediator* pTheArena);


	enum eState
	{
		SEARCHING_FOR_NEW_TARGET,		// Find another robot : 
		WAITING_FOR_NEW_TARGET_LOCATION,
		MOVING_TOWARD_TARGET,			// 	Move towards that robot : 
		ATTACKING_TARGET,				// If they are within 25 meters, they attack : 
		DOING_NOTING_IDLE				//	If nothing else, they are 
	};

	eState m_currentState = DOING_NOTING_IDLE;

	std::vector< iRobot* > vecRobots;

//	iRobotQuery* m_pTheMediator;

	iMediator* m_pTheArena;

	glm::vec2 m_TargetRobotLocation;

	// From the iMediator interface
	virtual sMessage getMessage(sMessage theMessage);
	virtual void recieveMessage(sMessage theMessage);


	virtual unsigned int getUniqueID(void);
private:
	unsigned int m_MyUniqueID;
	static unsigned int m_NextUniqueID;
};




