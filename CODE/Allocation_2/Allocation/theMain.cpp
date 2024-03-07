#include <iostream>

#include "cMesh.h"

// Changing the default allocation of new

cMesh* pBullet = NULL;

int main()
{
	// Aligned on 8 byte boundaries for 64 bit
	// 4 bytes for 32 bits
	std::cout << sizeof(cMesh) << " bytes" << std::endl;

	std::cout
		<< sizeof(cMesh) / 8 << "\t"
		<< (float)(sizeof(cMesh) / 8.0f) << "\t"
		<< sizeof(cMesh) % 8 << std::endl;

	cMesh* pMesh = new cMesh();

	cMesh newMesh;

	char* pX = new char('x');		// 1 byte

	delete pMesh;

	int* pShoeSize = new int(0);	// 32 bits 4 bytes

	cMesh* pMesh2 = new cMesh();


	cMesh* pMeshArray = new cMesh[100];

	int* pShoeSize2 = new int(0);	// 32 bits 4 bytes

	delete [] pMeshArray;


	//while (true)
	//{
	//	// Do game stuff

	//	// if player shot
	//	Shoot();

	//	// More stuff
	//	// if hit
	//	Hit();
	//}

	return 0;
}


void Shoot()
{
	pBullet = new cMesh();
	// Do stuff to start flying toward monster
	return;
}

void Hit()
{
	delete pBullet;

}

