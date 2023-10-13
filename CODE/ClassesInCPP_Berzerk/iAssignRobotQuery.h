#pragma once

#include "iRobotQuery.h"

class iAssignRobotQuery
{
public:
	~iAssignRobotQuery() {};

	virtual void set_RobotQuery(iRobotQuery* pTheArena) = 0;
};
