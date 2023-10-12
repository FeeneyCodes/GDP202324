#include "cArena.h"

#include "cRobotBuilder.h"
#include <iostream>

//#include "cWeapon.h"
//#include "cRayGun.h"

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
}

cArena::~cArena()
{
	delete this->pTheWeaponFactory;
}


void cArena::Init(int numRobots, int numPlayers)
{

	//iWeapon* pGun = this->pTheWeaponFactory->pMakeWeapon("Hello Kitty Murder Sling");
	//if ( ! pGun)
	//{
	//	// OOOOPS! 
	//}

	cRobotBuilder* pRobotBuilder = new cRobotBuilder();

	for ( unsigned int count = 0; count != numRobots; count++ )
	{
//		cRobot* pARobot = new cRobot();
		iRobot* pARobot = pRobotBuilder->BuildRobot("Robot");

//		pARobot->pTheWeapon = new cWeapon();
//		pARobot->pTheWeapon = this->pTheWeaponFactory->pMakeWeapon("Snazzy Rail Gun");

		this->vecRobots.push_back(pARobot);
	}

	for ( unsigned int count = 0; count != 2; count++ )
	{
//		iRobot* pARobot = new cSuperRobot();
		iRobot* pARobot = pRobotBuilder->BuildRobot("Super Robot");

//		pARobot->pTheWeapon = new cWeapon();
//		pARobot->pThe_2nd_Weapon = new cMissleLauncher();
//		pARobot->pTheWeapons[0] =this->pTheWeaponFactory->pMakeWeapon("Regular");
//		pARobot->pTheWeapons[1] = this->pTheWeaponFactory->pMakeWeapon("A Missle Launcher, please");
//
//		pARobot->pTheSword = new cSword();

		this->vecRobots.push_back(pARobot);

	}
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

void cArena::Update(double deltaTime)
{

	for (unsigned int index = 0; index != this->vecRobots.size(); index++)
	{
		this->vecRobots[index]->Attack();
		this->vecRobots[index]->Move(glm::vec2(10.0f, 10.0f));
		std::cout << std::endl;
	}


	for (unsigned int index = 0; index != this->vecPlayers.size(); index++)
	{
		this->vecPlayers[index]->Attack();
	}

	return;
}