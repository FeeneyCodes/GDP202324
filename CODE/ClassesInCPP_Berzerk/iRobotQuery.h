#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

class iRobotQuery
{
public:
	~iRobotQuery() {};

	// Is called by the robots to find other robots
	virtual glm::vec2 findClosestRobot(glm::vec2 whereIAm) = 0;
};
