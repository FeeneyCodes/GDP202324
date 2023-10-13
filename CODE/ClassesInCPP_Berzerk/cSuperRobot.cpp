#include "cSuperRobot.h"

#include <iostream>

cSuperRobot::cSuperRobot()
{
	std::cout << "SuperRobot!" << std::endl;
	// Make it stop....
}

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