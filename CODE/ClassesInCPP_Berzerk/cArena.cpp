#include "cArena.h"

#include "cRobotBuilder.h"
#include "iRobotPlacement.h"
#include "iAssignRobotQuery.h"
#include <iostream>
#include <thread>

//#include "cWeapon.h"
//#include "cRayGun.h"

// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float getRandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


void cArena::Init(int numRobots, int numPlayers,
				  float arenaSize)
{

	//iWeapon* pGun = this->pTheWeaponFactory->pMakeWeapon("Hello Kitty Murder Sling");
	//if ( ! pGun)
	//{
	//	// OOOOPS! 
	//}

	this->m_arenaSizeInMeters = arenaSize;

	cRobotBuilder* pRobotBuilder = new cRobotBuilder();

	for (unsigned int count = 0; count != numRobots; count++)
	{
//		cRobot* pARobot = new cRobot();
		iRobot* pARobot = pRobotBuilder->BuildRobot("Robot");

		// Tell the robot about me (the Arena)
		((iAssignRobotQuery*)pARobot)->set_RobotQuery(this);

		sMessage initLocation;
		initLocation.sData = "Set_Initial_Location";
		initLocation.vecData.push_back(glm::vec3(getRandomFloat(0.0f, this->m_arenaSizeInMeters),
												 getRandomFloat(0.0f, this->m_arenaSizeInMeters),
												 0.0f) );
		pARobot->recieveMessage(initLocation);

//		iRobotPlacement* pRobotPlacement = (iRobotPlacement*)pARobot;
//		pRobotPlacement->setInitalLocation(glm::vec2(getRandomFloat(0.0f, this->m_arenaSizeInMeters),
//													 getRandomFloat(0.0f, this->m_arenaSizeInMeters)));


// You would usually do this...
//		((iRobotPlacement*)pARobot)->setInitalLocation();

// Or this...
//		dynamic_cast<iRobotPlacement*>(pARobot)->setInitalLocation();


//		pARobot->pTheWeapon = new cWeapon();
//		pARobot->pTheWeapon = this->pTheWeaponFactory->pMakeWeapon("Snazzy Rail Gun");

		this->vecRobots.push_back(pARobot);
	}

	for (unsigned int count = 0; count != 2; count++)
	{
//		iRobot* pARobot = new cSuperRobot();
		iRobot* pARobot = pRobotBuilder->BuildRobot("Super Robot");

		// Tell the robot about me (the Arena)
		((iAssignRobotQuery*)pARobot)->set_RobotQuery(this);


		iRobotPlacement* pRobotPlacement = (iRobotPlacement*)pARobot;
		pRobotPlacement->setInitalLocation(glm::vec2(getRandomFloat(0.0f, this->m_arenaSizeInMeters),
													 getRandomFloat(0.0f, this->m_arenaSizeInMeters)));

//		pARobot->pTheWeapon = new cWeapon();
//		pARobot->pThe_2nd_Weapon = new cMissleLauncher();
//		pARobot->pTheWeapons[0] =this->pTheWeaponFactory->pMakeWeapon("Regular");
//		pARobot->pTheWeapons[1] = this->pTheWeaponFactory->pMakeWeapon("A Missle Launcher, please");
//
//		pARobot->pTheSword = new cSword();

		this->vecRobots.push_back(pARobot);

	}



	// Place the robots randomly in the arena.
	// Each robot is at least 10m away from each other







	for (unsigned int count = 0; count != numPlayers; count++)
	{
		cPlayer* pAPlayer = new cPlayer();

//		pAPlayer->pTheWeapon = new cRayGun();
		pAPlayer->pTheWeapon = this->pTheWeaponFactory->pMakeWeapon("Ray Gun");

		this->vecPlayers.push_back(pAPlayer);
	}

	// Get rid of the factory
	delete pRobotBuilder;

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
	this->pTheWeaponFactory = new cWeaponMaker();
	this->m_arenaSizeInMeters = 0.0f;
}

cArena::~cArena()
{
	delete this->pTheWeaponFactory;
}



void cArena::Update(double deltaTime)
{

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::cout << "**************************************************" << std::endl;

	for (unsigned int index = 0; index != this->vecRobots.size(); index++)
	{
		this->vecRobots[index]->Attack();
	//	this->vecRobots[index]->Move(glm::vec2(10.0f, 10.0f));
		this->vecRobots[index]->Update(deltaTime);
		std::cout << std::endl;
	}


//	for (unsigned int index = 0; index != this->vecPlayers.size(); index++)
//	{
//		this->vecPlayers[index]->Attack();
//	}

	return;
}


glm::vec2 cArena::findClosestRobot(glm::vec2 whereIAm)
{
	glm::vec2 closestRobotXY; 
	float distanceToClosestRobot = FLT_MAX;
	
	for ( iRobot* pRobot : this->vecRobots )
	{
		iRobotPlacement* pRobLocation = (iRobotPlacement*)pRobot;

		glm::vec2 loccationThisRobotXY = pRobLocation->getLocation();
		float currentDistance = glm::distance(loccationThisRobotXY, whereIAm);

		if (currentDistance < distanceToClosestRobot )
		{
			// It's the closest
			distanceToClosestRobot = currentDistance;
			closestRobotXY = loccationThisRobotXY;
		}
	}

	return closestRobotXY;
}
