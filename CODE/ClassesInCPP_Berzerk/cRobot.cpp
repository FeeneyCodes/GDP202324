#include "cRobot.h"

#include <iostream>

// constructor (c'tor)
cRobot::cRobot()
{
	std::cout << "cRobot::cRobot()" << std::endl;

	this->pos[0] = 0.0f;
	this->pos[1] = 0.0f;	// xy position

	this->bIsDead = true;

	this->dir[0] = 1.0f;
	this->dir[1] = 0.0f;

	this->bNoticedPlayer = false;

	// Get the robots
	//pArena->getTheVectorOfRobotsYo(

	// Set the ID
	this->m_MyUniqueID = cRobot::m_NextUniqueID;
	cRobot::m_NextUniqueID += 5;	// Ids go up by 5
//	cRobot::m_NextUniqueID += (rand() % 10);	// Up by a sort of random number

}

unsigned int cRobot::getUniqueID(void)
{
	return this->m_MyUniqueID;
}

//static 
unsigned int cRobot::m_NextUniqueID = 100;			// 1st robot has ID 100

void cRobot::setInitalLocation(glm::vec2 startingPosition)
{
	this->pos = startingPosition;
	return;
}

void cRobot::set_RobotQuery(iMediator* pTheArena)
{
	this->m_pTheArena = pTheArena;
	return;
}


glm::vec2 cRobot::getLocation(void)
{
	return this->pos;
}


// destructor (d'tor)
cRobot::~cRobot()
{
	std::cout << "cRobot::~cRobot()" << std::endl;
}

void cRobot::Attack(void)
{
	std::cout << "Robot Attacks!" << std::endl;
	this->pTheWeapon->Shoot(34);
	return;
}

//
//void cRobot::Move(glm::vec2 toWhere)
//{
//	std::cout << "Robot moves to " << toWhere.x << ", " << toWhere.y << std::endl;
//	return;
//}

void cRobot::Update(double deltaTime)
{
	switch (this->m_currentState)
	{
	case SEARCHING_FOR_NEW_TARGET:		// Find another robot : 
		// Ask the mediator
//		this->m_TargetRobotLocation = this->m_pTheMediator->findClosestRobot(this->pos);
//		
//		std::cout << "Am moving towards a robot at "
//			<< this->m_TargetRobotLocation.x << ", "
//			<< this->m_TargetRobotLocation.y << std::endl;
		{
			sMessage getNewTarget;
			getNewTarget.sData = "Find the closest robot";
//			getNewTarget.sData = "findClosestRobot(0.0, 10.0f, 10.0)";
			getNewTarget.vecData.push_back(glm::vec3(this->pos, 0.0f));
			getNewTarget.UniqueIDofSender = this->getUniqueID();

			// Blocking (syncronous) call:
			sMessage ArenaResponse = this->m_pTheArena->getMessage(getNewTarget);
		}

		// 
		this->m_currentState = MOVING_TOWARD_TARGET;
		break;
	case MOVING_TOWARD_TARGET:			// 	Move towards that robot : 
		break;
	case ATTACKING_TARGET:				// If they are within 25 meters, they attack : 
		break;
	case DOING_NOTING_IDLE:				//	If nothing else, they are 
		// Switch to looking for a target
		this->m_currentState = SEARCHING_FOR_NEW_TARGET;
		break;
	}


	return;
}


	// From the iMediator interface
sMessage cRobot::getMessage(sMessage theMessage)
{



	return theMessage;
}

void cRobot::recieveMessage(sMessage theMessage)
{
	return;
}


