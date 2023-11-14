// Here's an example of how you might take a large scene and "slice" it up
// to see what AABB the triangles are in
#include "cAABB.h"

#include "sTriangle.h"

#include <vector>
#include <iostream>

#include "Ply_File_Loader/CPlyFile5nt.h"


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
		 itTri != vecTriangles.end(); itTri++)
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

bool IsTriangleInAABB(sTriangle_A theTri, cAABB* pAABB )
{
	if ( theTri.getSideLentgh(0) > pAABB->getExtentsXYZ().x )
	{
		// Subdivide triangles 
		// (suggestion: use the centre to split the triangle)
		sTriangle_A tri[3];
		for ( int triIndex = 0; triIndex != 3; triIndex++ )
		{ 
			// Make the three triangles
			if ( IsTriangleInAABB(tri[triIndex], pAABB) )
			{
				return true;
			}
		}
	}
	
//	return false;

	// The actual test is here...
}


int main()
{
	CPlyFile5nt plything;

	std::wstring sError;
//	if ( ! plything.OpenPLYFile2(L"Flat_100x100_plane.ply", sError) )
	if ( ! plything.OpenPLYFile2(L"BirdOfPrey_xyz_n_rgba_centred_facing_on_positive_Z.ply", sError) )
	{
		std::cout << "didn't load it. Exiting." << std::endl;
	}


	std::map< unsigned int /*index*/, cAABB* > mapAABBS_by_ID;

	// JUST check vertices to see how many aabbs we have
	for ( unsigned int index = 0; index != plything.GetNumberOfVerticies(); index++ )
	{

		PlyVertex vert = plything.getVertex_at(index);


		glm::vec3 vTestVert = glm::vec3(vert.xyz.x, vert.xyz.y, vert.xyz.z);

	//	Bird of Prey is: 	510 x 154 x 345
	// 
	//	so will fit into a box:  1000 x 1000 x 1000
	//
	//	If we want 10 boxes per side, 
	// 
	//	Each AABB is 100x100x100 in dimensions 


		float extent = 100.0f;
		glm::vec3 extents = glm::vec3(extent, extent, extent);


		unsigned int AABB_ID = cAABB::static_getLocationIndex(vTestVert, extents);

//		std::cout << vert.xyz.x << ", " << vert.xyz.y << ", " << vert.xyz.z << " : ";
//		std::cout << AABB_ID << std::endl;

		// We are creating these here, but we also need to load the AABB with the 
		//	appropriate inforamtion
		// 
		// This [] will create or overwrite the item
		
		// Is there already an AABB? 
		std::map< unsigned int /*index*/, cAABB* >::iterator itAABB = mapAABBS_by_ID.find(AABB_ID);

		if ( itAABB == mapAABBS_by_ID.end() )
		{
			// No, there isn't an AABB there
			mapAABBS_by_ID[AABB_ID] = new cAABB();
		}

	}//for ( unsigned int index

	std::cout << mapAABBS_by_ID.size() << std::endl;








	std::map< unsigned int /*index*/, cAABB* > mapAABBS_by_ID_2;

	// Pre-create the AABB
	float extent = 1000.0f;
	unsigned int numAABBsPerSide = 10;
	//
	// NOTE: This allows negative locations
	// (you might not want that)
	// 
	for (  int x = -5; x != 5; x++ )
	{
		for ( int y = -5; y != 5; y++)
		{
			for ( int z = -5; z != 5; z++)
			{
				cAABB* pAABB = new cAABB();

				pAABB->minXYZ.x = x * extent;
				pAABB->minXYZ.y = y * extent;
				pAABB->minXYZ.z = z * extent;

				pAABB->maxXYZ.x = pAABB->minXYZ.x + extent;
				pAABB->maxXYZ.y = pAABB->minXYZ.y + extent;
				pAABB->maxXYZ.z = pAABB->minXYZ.z + extent;

				glm::vec3 extents = glm::vec3(extent, extent, extent);

				unsigned int AABB_ID = cAABB::static_getLocationIndex(pAABB->minXYZ, extents);

				mapAABBS_by_ID_2[AABB_ID] = pAABB;

			}
		}
	}


// JUST check vertices to see how many aabbs we have
	for (unsigned int index = 0; index != plything.GetNumberOfVerticies(); index++)
	{

		PlyVertex vert = plything.getVertex_at(index);


		glm::vec3 vTestVert = glm::vec3(vert.xyz.x, vert.xyz.y, vert.xyz.z);

	//	Bird of Prey is: 	510 x 154 x 345
	// 
	//	so will fit into a box:  1000 x 1000 x 1000
	//
	//	If we want 10 boxes per side, 
	// 
	//	Each AABB is 100x100x100 in dimensions 


		float extent = 100.0f;
		glm::vec3 extents = glm::vec3(extent, extent, extent);
//		Bird of Prey
//		510 x 154 x 345

		unsigned int AABB_ID = cAABB::static_getLocationIndex(vTestVert, extents);

		mapAABBS_by_ID_2[AABB_ID]->vecVerticesInside.push_back(vTestVert);

//		std::cout << vert.xyz.x << ", " << vert.xyz.y << ", " << vert.xyz.z << " : ";
//		std::cout << AABB_ID << std::endl;

		// We are creating these here, but we also need to load the AABB with the 
		//	appropriate inforamtion
		// 
		// This [] will create or overwrite the item

		// Is there already an AABB? 
//		std::map< unsigned int /*index*/, cAABB* >::iterator itAABB = mapAABBS_by_ID_2.find(AABB_ID);


//		if (itAABB == mapAABBS_by_ID.end())
//		{
//			// No, there isn't an AABB there
//			mapAABBS_by_ID[AABB_ID] = new cAABB();
//		}

	}//for ( unsigned int index

	std::cout << mapAABBS_by_ID_2.size() << std::endl;

	for ( std::map< unsigned int /*index*/, cAABB* >::iterator itAABB = mapAABBS_by_ID_2.begin();
		 itAABB != mapAABBS_by_ID_2.end(); itAABB++ )
	{
		if ( ! itAABB->second->vecVerticesInside.empty() )
		{
			std::cout << itAABB->second->getLocationIndex()
				<< " has " << itAABB->second->vecVerticesInside.size()
				<< " vertices inside it." << std::endl;
		}
	}



	return 0;
}





