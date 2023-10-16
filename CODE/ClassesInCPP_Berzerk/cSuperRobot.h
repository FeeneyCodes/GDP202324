#pragma once

// Super Robot is just like the robot, but 
//	it's faster (I quess) or maybe has a differnet mesh
// BUT it can use a missle launcher or a regular weapon
//  and it dual weilds. 

//#include "cMissleLanucher.h"
#include "iRobot.h"
#include "iWeapon.h"
#include "cSword.h"
#include "iRobotPlacement.h"
#include "iAssignRobotQuery.h"
#include "iRobotQuery.h"

class cSuperRobot : 
	public iRobot, 
	public iRobotPlacement,
	public iAssignRobotQuery

{
public:
	cSuperRobot();
	virtual ~cSuperRobot() {};

	// Has two weapons...
	iWeapon* pTheWeapons[2];
	// ... and a sword!
	cSword* pTheSword;

	// From iRobot interface
	virtual void Attack(void);
//	virtual void Move(glm::vec2 toWhere);
	virtual void Update(double deltaTime);

	glm::vec2 position;	// xy position
	bool bIsDead;
	glm::vec2 dir;	// direction
	bool bNoticedPlayer;

	// From iRobotPlacement interface
	virtual void setInitalLocation(glm::vec2 startingPosition);
	virtual glm::vec2 getLocation(void);

	virtual void set_RobotQuery(iRobotQuery* pTheArena);


	iRobotQuery* m_pTheMediator;

	// From the iMediator interface
	virtual sMessage getMessage(sMessage theMessage);
	virtual void recieveMessage(sMessage theMessage);

	virtual unsigned int getUniqueID(void);
private:
	unsigned int m_MyUniqueID;
	static unsigned int m_NextUniqueID;
};

