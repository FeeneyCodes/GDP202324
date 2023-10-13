#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

// "Where's the nearest robot, yo?"
// (x,y,z)   <-- I'm here
//
// "The robot is here, yo"
// (x,y,z)
//
// "Missle exploded here"
// (x,y,z)


struct sMessage
{
	unsigned int UniqueIDofSender;
	std::string sData;					//"Where's the nearest robot, yo?"
	std::vector<glm::vec3> vecData;		// (x,y,z)   <-- I'm here
};

class iMediator
{
public:
	virtual sMessage getMessage(sMessage theMessage) = 0;
	virtual void recieveMessage(sMessage theMessage) = 0;
};
