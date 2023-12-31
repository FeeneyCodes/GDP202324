#include "cRailGun.h"
#include <iostream>

bool cRailGun::Shoot(int numBullets)
{
	std::cout << "Rail gun goes PEYEW!" << std::endl;
	return true;
}

glm::vec2 cRailGun::getPosition(void)
{
	return this->position;
}

void cRailGun::setPosition(glm::vec2 newPosition)
{
	this->position = newPosition;
	return;
}

glm::vec2 cRailGun::getVelocity(void)
{
	return this->velocity;
}

void cRailGun::setVelocity(glm::vec2 newVelocity)
{
	this->velocity = newVelocity;
	return;
}

bool cRailGun::get_isShot(void)
{
	return this->isShot;
}

void cRailGun::set_IsShot(bool bIsShot)
{
	this->isShot = bIsShot;
	return;
}