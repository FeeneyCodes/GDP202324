#include <iostream>

#include "cMesh.h"

// Changing the default allocation of new


// DANGER!!! 
// This will overload new() for EVERYTHING
//void* operator new(size_t size)
//{
//	std::cout << "Oh no... " << std::endl;
//
//	return 0;
//}


int main()
{
//	cMesh* pMyMesh = ::new cMesh();


	int x = sizeof(cMesh);

	cMesh::InitMemory();

	cMesh* pMesh = new cMesh();

	pMesh->drawPosition = glm::vec3(1.05f, 123.3f, 18.33f);

	unsigned int rn = RAND_MAX;

	cMesh* pMesh2 = new cMesh();

	pMesh2->drawPosition = glm::vec3(3.0f, 2.0f, 1.0f);

	delete pMesh2;


	cMesh* pMesh3 = new cMesh();
	pMesh3->drawPosition = glm::vec3(15.0f, 16.0f, 17.0f);


	delete pMesh;

//	cMesh* pHey = new cMesh[3];

//	cMesh* pM = (cMesh*)malloc(sizeof(unsigned int));
	//pM->bIsWireframe = true;
//	pM->drawPosition = glm::vec3(0.0f, 14.0f, 46.4f);



	return 0;
}

