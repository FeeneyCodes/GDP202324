#include "cMesh.h"

//static 
void cMesh::InitMemory(void)
{
	// Allocate space for maxumum number of meshes
	// (here it's 10,000)
	// 
	// NOTE: if we juse use "new" it will call the cMesh "new[]" operator
	//	(which will be recursive hell)
	// 
	// So we are telling to use the "global" (i.e. regular) new
	// by using "::new"
	// 
	cMesh::m_TheMeshes = ::new cMesh[cMesh::MAX_NUMBER_MESHES];

	// Clear the memory here to all zeros
	// This will help me find "available" memory
	memset(cMesh::m_TheMeshes, 0, sizeof(cMesh) * cMesh::MAX_NUMBER_MESHES);

	cMesh::m_MeshInUse = ::new bool[cMesh::MAX_NUMBER_MESHES];
	
	// Setting this to zero as well (false is zero)
	memset(cMesh::m_MeshInUse, false, sizeof(bool) * cMesh::MAX_NUMBER_MESHES);
	


	return;
}


//static 
cMesh* cMesh::m_TheMeshes = nullptr;
bool* cMesh::m_MeshInUse = nullptr;
unsigned int cMesh::m_numMeshInUse = 0;



//static 
void* cMesh::operator new(size_t size)
{
	// Is there any memory left? 
	if (cMesh::m_numMeshInUse >= MAX_NUMBER_MESHES )
	{
		// Nope. Sorry.
		return nullptr;
	}


	// Find the 1st empty "slot" that I can put a mesh
	// 
	// cMesh* m_pTheMeshes = new cMesh[cMesh::MAX_NUMBER_MESHES];	
	// 
	//static const unsigned int MAX_NUMBER_MESHES = 10'000;

	for ( unsigned int index = 0; index != MAX_NUMBER_MESHES; index++ )
	{
		if ( ! cMesh::m_MeshInUse[index] )
		{
			// We've found a "slot" to place this mesh
			cMesh::m_MeshInUse[index] = true;

			// Get address of this location:
			cMesh* pMeshAllocated = &(cMesh::m_TheMeshes[index]);
			cMesh::m_numMeshInUse++;
			return pMeshAllocated;
		}
	}

	while (true)
	{
		// Pick a random index 
		// RAND_MAX is 32K, so if we have MORE than 32K, 
		//	some of the locations will never be picked
		unsigned int textIndex = rand() % MAX_NUMBER_MESHES;

		// In use? 
		if (!cMesh::m_MeshInUse[textIndex])
		{
			// We've found a "slot" to place this mesh
			cMesh::m_MeshInUse[textIndex] = true;

			// Get address of this location:
			cMesh* pMeshAllocated = &(cMesh::m_TheMeshes[textIndex]);
			return pMeshAllocated;
		}
	};


	return nullptr;		// Or 0 or NULL
}

//static 
void cMesh::operator delete (void* ptr)
{
	// ptr points to the object we are deleting.
	
	// We need to find the index offset based on this pointer

	// How far from the cMesh::m_TheMeshes[0]

	unsigned int pMesh0Address = (unsigned int)&(cMesh::m_TheMeshes[0]);
	
	// this is how many bytes from the 0th location this object is
	unsigned int pointerOffset = (unsigned int)ptr - pMesh0Address;

	// What's the index
	unsigned int indexOfObject = pointerOffset / sizeof(cMesh);

	cMesh::m_MeshInUse[indexOfObject] = false;

	cMesh::m_numMeshInUse--;

	return;
}















// Michael is NOT going to do these

//static 
//void* cMesh::operator new[](std::size_t count)
//{
//	// TODO: Insert amazing array allocation here
//	return nullptr;
//}
//
////static 
//void cMesh::operator delete[](void* ptr)
//{
//	// TODO: Insert amazing array deallocation here
//	return;
//}

