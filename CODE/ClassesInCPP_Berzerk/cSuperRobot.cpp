#include "cSuperRobot.h"

#include <iostream>

cSuperRobot::cSuperRobot()
{
	std::cout << "SuperRobot!" << std::endl;
	// Make it stop....
		// Set the ID
	this->m_MyUniqueID = cSuperRobot::m_NextUniqueID;
	cSuperRobot::m_NextUniqueID += 5;	// Ids go up by 5
}


unsigned int cSuperRobot::getUniqueID(void)
{
	return this->m_MyUniqueID;
}

//static 
unsigned int cSuperRobot::m_NextUniqueID = 90000;			// 1st robot has ID 90000

void cSuperRobot::setInitalLocation(glm::vec2 startingPosition)
{
	this->position = startingPosition;
	return;
}

glm::vec2 cSuperRobot::getLocation(void)
{
	return this->position;
}

void cSuperRobot::Attack(void)
{
	std::cout << "SuperRobot Attacks! " << std::endl;
	
	this->pTheWeapons[0]->Shoot(75);
	this->pTheWeapons[1]->Shoot(75);

	this->pTheSword->Slash(100.0f);
	return;
}

void cSuperRobot::set_RobotQuery(iRobotQuery* pTheArena)
{
	this->m_pTheMediator = pTheArena;
	return;
}


//void cSuperRobot::Move(glm::vec2 toWhere)
//{
//	std::cout << "cSuperRobot moves to " << toWhere.x << ", " << toWhere.y << std::endl;
//	std::cout << "(and makes WAY more noise because it's way bigger, or something" << std::endl;
//	return;
//}

void cSuperRobot::Update(double deltaTime)
{


	return;
}


	// From the iMediator interface
sMessage cSuperRobot::getMessage(sMessage theMessage)
{



	return theMessage;
}

void cSuperRobot::recieveMessage(sMessage theMessage)
{
	return;
}