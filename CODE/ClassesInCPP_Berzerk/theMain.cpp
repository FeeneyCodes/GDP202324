//#include "cPlayer.h"
//#include "cRobot.h"
//#include "cBullet.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

//#include "cWeapon.h"
//#include "cRayGun.h"
//#include "cMissleLanucher.h"

#include "cArena.h"

bool isCloseEnoughToHit(glm::vec2 a, glm::vec2 b)
{
	// Distance between two points
	float dist = glm::distance(a, b);

	const float SOME_SMALL_AMOUNT = 1.0f;
	if ( dist < SOME_SMALL_AMOUNT )
	{
		return true;
	}
	return false;

}


int main()
{
//	cArena* pTheGame = new cArena();
	cArena* pTheGame = cArena::getTheArena();
//	cArena* pTheGame2 = cArena::getTheArena();


	pTheGame->Init(3, 1);


	std::cout << std::endl;
	std::cout << "Game loop starting..." << std::endl;

	while ( true )
	{

		pTheGame->Update(0.0f);
	}

	



//	pB->Shoot();	// Weapon
//	pR->Shoot();	// RayGun

	int x = 0;
	float b = (float)x;			//	static_cast<float>(x);

// You CAN do this, but likely shouldn't...
//	( (cWeapon)b ).AmmoLeft(); 
//	(int)b.Shoot();

//
//	cWeapon* p = new cRayGun();
//
//	delete p;
//
//
////	cPlayer bob;		// Stack
//
////	cPlayer* pBob = new cPlayer();		// HEAP
//
//	const unsigned int NUM_ROBOTS = 8;
//
//	// Dynamic array, but appears as a stack based array
////	cRobot* theRobots = new cRobot[NUM_ROBOTS];	// Heap
//	std::vector< cRobot* > vec_pRobots;
//	for ( unsigned int count = 0; count != NUM_ROBOTS; count++ )
//	{
//		cRobot* pNewRobot = new cRobot();
//
//		vec_pRobots.push_back(pNewRobot);
//	}
//
//
////	cRobot theRobots[NUM_ROBOTS];			// Stack
//
//	// Place player and robots...
//
//	while (true)
//	{
//		// ASsume player presses "shoot" and a director	
//		std::cout << "Player shoots..." << std::endl;
//
//
//		std::cout << "Player shot!" << std::endl;
//
//
//
//
//		//if ( pBob->bullet.isShot )
//		//{
//		//	// Move bullet, etc.
//
//		//	for ( unsigned int index = 0; index != NUM_ROBOTS; index++ )
//		//	{
//		//		if ( isCloseEnoughToHit( vec_pRobots[index]->pos, pBob->bullet.position) )
//		//		{
//		//			// It's a hit
//
//		//			vec_pRobots[index]->bIsDead = true;
//
//		//			//pBob->bullet.isShot = false;
//		//		}
//		//	}
//
//		//}//if ( pBob->bullet.isShot )
//	}
//
//
//
////	delete pRobots;		// YIKES! GOTCHA!! BEWARE
////	delete [] pRobots;
	

	return 0;
}
