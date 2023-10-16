#include "cArena.h"

#include "cRobotBuilder.h"
#include "iRobotPlacement.h"
#include "iAssignRobotQuery.h"
#include <iostream>
#include <thread>
#include <iostream>


#include "cArena_Implementation.h"

// Sort of like doing this...
float cat_weight;



void cArena::Init(int numRobots, int numPlayers,
				  float arenaSize)
{
	this->m_pTheAcualArena->Init(numRobots, numPlayers, arenaSize);

	return;
}

//static variables have to be assigned this way.
cArena* cArena::m_pTheOneAndOnlyArena = NULL;


//static 
cArena* cArena::getTheArena(void)
{
	if (cArena::m_pTheOneAndOnlyArena == NULL)
	{
		cArena::m_pTheOneAndOnlyArena = new cArena();
	}
	return cArena::m_pTheOneAndOnlyArena;
}

cArena::cArena()
{
	std::cout << "cArena created." << std::endl;

	// Make the implementation
	this->m_pTheAcualArena = new cArena_Implementation();
}

cArena::~cArena()
{
	delete this->m_pTheAcualArena;
}



void cArena::Update(double deltaTime)
{
	this->m_pTheAcualArena->Update(deltaTime);

	return;
}


glm::vec2 cArena::findClosestRobot(glm::vec2 whereIAm)
{
	return this->m_pTheAcualArena->findClosestRobot(whereIAm);
}


	// From the iMediator interface
sMessage cArena::getMessage(sMessage theMessage)
{
	return this->m_pTheAcualArena->getMessage(theMessage);
}

void cArena::recieveMessage(sMessage theMessage)
{
	this->m_pTheAcualArena->recieveMessage(theMessage);
	return;
}