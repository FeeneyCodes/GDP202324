#pragma once

#include "cPlayer.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include "iRobotQuery.h"
#include "iMediator.h"

//#include "cArena_Implementation.h"

// Forward declaration (DANGER WILL ROBINSON!!)
class cArena_Implementation;

class cArena :
	public iRobotQuery,
	public iMediator
{
public:
	~cArena();
	// Assume arena is square. 
	void Init(int numRobots, int numPlayers, 
			  float arenaSizeInMeters = 250.0f);

	void Update(double deltaTime);

	// Any static variable is available even if the object
	//	isn't created yet;
	static cArena* getTheArena(void);

	// from the iRobotQuery interface
	virtual glm::vec2 findClosestRobot(glm::vec2 whereIAm);

	// From the iMediator interface
	virtual sMessage getMessage(sMessage theMessage);
	virtual void recieveMessage(sMessage theMessage);

private:
	cArena();
	static cArena* m_pTheOneAndOnlyArena;

	// The actual Arena implementation class
	cArena_Implementation* m_pTheAcualArena;
};
