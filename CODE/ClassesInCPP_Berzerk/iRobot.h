#pragma once
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include "iMediator.h"

class iRobot :
	public iMediator
{
public:
	virtual ~iRobot() {};

	virtual void Attack(void) = 0;

	virtual unsigned int getUniqueID(void) = 0;


// Robots will move themselves
//	virtual void Move(glm::vec2 toWhere) = 0;

	virtual void Update(double deltaTime) = 0;

	virtual sMessage getMessage(sMessage theMessage) = 0;
	virtual void recieveMessage(sMessage theMessage) = 0;
};

