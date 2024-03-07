#include <iostream>

#include "cMesh.h"

// Changing the default allocation of new

unsigned int MAX_NUMBER_OF_MESHES = 10000;
cMesh* pLotsOfMeshes = NULL;

int main()
{
	// Create a large pool of memory just for cMesh
	
	pLotsOfMeshes = new cMesh[MAX_NUMBER_OF_MESHES];

	int x = sizeof(pLotsOfMeshes);

	cMesh y[10];
	int z = sizeof(y);

//	delete pLotsOfMeshes;
	delete [] pLotsOfMeshes;	// <- the runtime KNOWS how big...

	cMesh* pMyMesh = new cMesh();

	delete pMyMesh;


	// malloc() -- this is what C uses
	cMesh* pMyOtherMesh = (cMesh*)malloc(sizeof(cMesh));


	free(pMyOtherMesh);


	return 0;
}