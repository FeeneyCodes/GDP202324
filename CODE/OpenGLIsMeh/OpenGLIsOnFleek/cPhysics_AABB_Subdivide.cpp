// Here's an example of how you might take a large scene and "slice" it up
// to see what AABB the triangles are in
#include "cAABB.h"

#include "sTriangle.h"

#include <vector>


// This 
void ProcessABBBsFromScene(cAABB& parentAABB,				// The parent containing the AABBS
						   std::vector<sTriangle_A>& vecTriangles,	// Triangles in model
						   glm::ivec3 NumberBoxesPerSide,	// eg: 10x10x10 or whatever
						   glm::vec3 extentLength)			// How big is the ENTIRE 'parent' AABB
															// (i.e. how big are each 'child' AABB
{
	// Calculate how big the AABBs are
	// Decide if we want to pre-populate all the AABBs? 
	//	Or only ones that have triangles? 

	// Also, where's the origin of this root AABB
	// (Let's assume it's at the origin 0,0,0)
	parentAABB.minXYZ.x = -extentLength.x / 2.0f;
	parentAABB.minXYZ.y = -extentLength.y / 2.0f;
	parentAABB.minXYZ.z = -extentLength.z / 2.0f;

	parentAABB.maxXYZ.x = extentLength.x / 2.0f;
	parentAABB.maxXYZ.y = extentLength.y / 2.0f;
	parentAABB.maxXYZ.z = extentLength.z / 2.0f;

	glm::vec3 childAABBExtents;
	childAABBExtents.x = extentLength.x / NumberBoxesPerSide.x;
	childAABBExtents.y = extentLength.y / NumberBoxesPerSide.y;
	childAABBExtents.z = extentLength.z / NumberBoxesPerSide.z;

	// Are we going to make ALL the AABBs? (that's fine)
	// *** Are we only going to make the AABBs that have triagles? 


	for (std::vector<sTriangle_A>::iterator itTri = vecTriangles.begin();
		 itTri != vecTriangles.end(); itTri++ )
	{
		unsigned int AABB_ID_V1 = cAABB::static_getLocationIndex(itTri->vertices[0],
															     childAABBExtents);
		// Is that AABB already there (in the map)
		std::map< unsigned int /*index*/, cAABB* >::iterator itAABB_v1 = parentAABB.vecChild_pAABBs.find(AABB_ID_V1);
		
		if (itAABB_v1 == parentAABB.vecChild_pAABBs.end())
		{
			// Nope. So make one.
			cAABB newAABB;
			//newAABB.minXYZ 
		}
	}


	return;
}


void DOIt(void)
{
	cAABB rootAABB;
	std::vector<sTriangle_A> vecTriangles;	// And loaded with Hogwarts
	glm::ivec3 NumberBoxesPerSide = glm::ivec3(10, 10, 10);
	glm::vec3 extentLength = glm::vec3(1'000'000.0f, 1'000'000.0f, 1'000'000.0f);
	ProcessABBBsFromScene(rootAABB, vecTriangles, NumberBoxesPerSide, extentLength);
}





