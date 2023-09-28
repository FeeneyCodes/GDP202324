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
}

// destructor (d'tor)
cRobot::~cRobot()
{
	std::cout << "cRobot::~cRobot()" << std::endl;
}