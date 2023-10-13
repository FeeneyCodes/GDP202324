#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

// Used the the Arena to initially set the robots.
// Also used to get the current location of the robots. 

class iRobotPlacement
{
public:
	virtual ~iRobotPlacement() {};

	// Intial placement when creating.
	virtual void setInitalLocation(glm::vec2 startingPosition) = 0;

	// Gets the current location of the robot
	virtual glm::vec2 getLocation(void) = 0;
};
