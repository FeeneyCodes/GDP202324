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

// Robots will move themselves
//	virtual void Move(glm::vec2 toWhere) = 0;

	virtual void Update(double deltaTime) = 0;

};

