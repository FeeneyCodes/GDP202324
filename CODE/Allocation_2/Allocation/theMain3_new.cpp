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


	cMesh::InitMemory();

	cMesh* pMesh = new cMesh();

	pMesh->drawPosition = glm::vec3(1.05f, 123.3f, 18.33f);

	

//	cMesh* pMesh2 = pMesh;

	delete pMesh;





	return 0;
}

