#include "cGameObject.h"
#include "Utilities.h"	// For getRandFloat
#include <vector>
#include "GlobalStuff.h"

// variable is NOT in this file, but it's somewhere
// (i.e. the linker will "link" to it)
extern unsigned int g_DebugSphereID;
extern unsigned int g_1SidedQuadID;
extern unsigned int g_2SidedQuadID;

#include <glm/glm.hpp>

cGameObject* PlaceObject( std::string objectName, glm::vec3 location );
cGameObject* PlaceObject( std::string objectName, glm::vec3 location, glm::vec3 rotation );
cGameObject* PlaceObject( std::string objectName, glm::vec3 location, glm::vec3 rotation, float scale );

bool LoadObjectsIntoScene( std::vector< cGameObject* > &vec_pGOs )
{



	float gridLimit = 30.0f;
	float gridStep = 15.0f;

	for ( float x = -gridLimit; x <= (gridLimit + gridStep); x += gridStep )
	{
		for ( float y = -gridLimit; y <= (gridLimit + gridStep); y += gridStep )
		{
			for ( float z = -gridLimit; z <= (gridLimit + gridStep); z += gridStep )
			{

				cGameObject* pObject = new cGameObject();
				pObject->bIsUpdatedByPhysics = false;	// 
				pObject->bIsWireframe = false;
				// Pick a semi-random location
				pObject->x = x;		// getRandFloat(-30.0f, 30.0f);
				pObject->y = y;		// getRandFloat(-30.0f, 30.0f);
				pObject->z = z;		// getRandFloat(-30.0f, 30.0f);

				//pObject->radius = 0.5f;
				pObject->scale = getRandFloat( 0.5f, gridStep );
				pObject->scale = 10.0f;

				pObject->meshName = "cow_N.ply";
//				pObject->meshName = "Utah_Teapot.ply";
//				pObject->qOrientaion = glm::quat( glm::vec3( -3.14/2.0f, 0.0f, 0.0f ) );

				// Pick a random rotation
//				pSphere->post_Rot_Y = getRandFloat( 0.0f, 10.0f );
//
		//		pSphere->meshName = "Sphere.ply";
				// Pick a bunch of random models
//				switch ( static_cast<int>(getRandFloat(0, 20 )) )
//				{
//				case 0:
//					pSphere->meshName = "bun_zipper_res3_XYZ_N.ply";
//					break;
//				case 1:
//					pSphere->meshName = "su47_XYZ_N.ply";
//					break;
//				case 2:
//					pSphere->meshName = "dolphin_XYZ_N.ply";
//					break;
//				case 3:
//					pSphere->meshName = "cow_N.ply";
//					break;
//				case 4:
//					pSphere->meshName = "tie_withNormals.ply";
//					break;
//				case 5:
//					pSphere->meshName = "bun_zipper_N.ply";
//					break;
//				case 6:
//					pSphere->meshName = "bun_zipper_res2_N.ply";
//					break;
//				case 7:
//					pSphere->meshName = "bun_zipper_N.ply";
////					pSphere->meshName = "bun_zipper_res4_N.ply";
////					break;
//				case 8:
//					pSphere->meshName = "Cube_1x1x1_N.ply";
//					break;
//				case 9:
//					pSphere->meshName = "Cube2.ply";
//					// Make slightly smaller cubes...
//					pSphere->scale = getRandFloat( 0.2f, 3.0f );
//					break;
//				case 10:
//				default:
////					pSphere->meshName = "dragon_vrip_res2_N.ply";
//					pSphere->meshName = "dolphin_XYZ_N.ply";
//					break;
//				}//switch ( static_cast<int>(getRandFloat(0, 20 )) )
		//
				//// All dragons...
				//pSphere->meshName = "dragon_vrip_res2_N.ply";

				vec_pGOs.push_back( pObject );

			}//for ( float z =...
		}//for ( float y =...
	}//for ( float x =...


	//for ( float z = -40.0f; z <= 41.0; z += 10.0f )
	//{
	//	cGameObject* pThing = new cGameObject();
	//	pThing->bIsUpdatedByPhysics = false;	// 
	//	pThing->bIsWireframe = false;
	//	pThing->x = 0.0f;	pThing->y = 0.0f;	pThing->z = z;
	//	pThing->post_Rot_X = -3.14f/2.0f;
	//	pThing->meshName = "Utah_Teapot.ply";
	//	pThing->scale = 10.0f;			// Ground is 50.0x50.0 
	//	vec_pGOs.push_back( pThing );
	//}


	{
		cGameObject* pGround = new cGameObject();
		pGround->bIsUpdatedByPhysics = false;	// 
		pGround->bIsWireframe = false;
		pGround->x = 0.0f;	pGround->y = -30.0f;	pGround->z = 0.0f;
	//	pGround->meshName = "Ground_XYZ_N_Rot_Y_is_up.ply";
		pGround->meshName = "Seafloor2_N.ply";
		pGround->scale = 250.0f;			// Ground is 50.0x50.0 
		vec_pGOs.push_back( pGround );
	}
	//{	// Same, but wireframe so it can be seen
	//	cGameObject* pGround = new cGameObject();
	//	pGround->bIsUpdatedByPhysics = false;	// 
	//	pGround->bIsWireframe = true;
	//	pGround->bUseDebugColours = true;
	//	pGround->x = 0.0f;	pGround->y = -29.9f;	pGround->z = 0.0f;
	//	pGround->meshName = "Seafloor2_N.ply";
	//	pGround->scale = 250.0f;			// Ground is 50.0x50.0 
	//	vec_pGOs.push_back( pGround );
	//}

	{	// Add a "debug sphere"
		cGameObject* pSphere = new cGameObject();
		pSphere->bIsUpdatedByPhysics = false;	// 
		pSphere->bIsWireframe = true;
		// Pick a semi-random location
		pSphere->x = 0.0f;	pSphere->y = 0.0f;	pSphere->z = 0.0f;

		pSphere->scale = 1.0f;	// Unit sphere (0.5 units raduis)

	//	pSphere->meshName = "Sphere_InvertedNormals.ply";
		pSphere->meshName = "Isoshphere_xyz_InvertedNormals.ply";

		pSphere->bIsVisible = false;

		::g_DebugSphereID = pSphere->getID();
		vec_pGOs.push_back( pSphere );
	}


	{	// Add a 1 sided quad
		cGameObject* p1SidedQuad = new cGameObject();
		p1SidedQuad->bIsUpdatedByPhysics = false;	// 
		p1SidedQuad->bIsWireframe = true;
		p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
		p1SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
		p1SidedQuad->meshName = "1x1_2Tri_Quad_1_Sided_XY_Plane.ply";
		p1SidedQuad->texture00Name = "smoketex.bmp";
		p1SidedQuad->bIsVisible = false;
		p1SidedQuad->bIsImposter = true;
		::g_1SidedQuadID = p1SidedQuad->getID();
		vec_pGOs.push_back( p1SidedQuad );
	}
	{	// Add a 2 sided quad
		cGameObject* p2SidedQuad = new cGameObject();
		p2SidedQuad->bIsUpdatedByPhysics = false;	// 
		p2SidedQuad->bIsWireframe = true;
		p2SidedQuad->x = 0.0f;	p2SidedQuad->y = 0.0f;	p2SidedQuad->z = 0.0f;
		p2SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
		p2SidedQuad->meshName = "1x1_2Tri_Quad_2_Sided_XY_Plane.ply";
		p2SidedQuad->bIsVisible = false;
		p2SidedQuad->bIsImposter = true;
		p2SidedQuad->texture00Name = "smoketex.bmp"; 
		::g_2SidedQuadID = p2SidedQuad->getID();
		vec_pGOs.push_back( p2SidedQuad );
	}

	for ( std::vector<cGameObject*>::iterator itpGO = vec_pGOs.begin();
		  itpGO != vec_pGOs.end(); itpGO++ )
	{
//		cGameObject* pTempGO;
//		pTempGO = *itpGO;		
//		pTempGO->alpha;
//		(*pTempGO).alpha;

//		(*itpGO)->alpha = getRandFloat( 0.1f, 0.75f );
		(*itpGO)->alpha = 1.0f;
	}


	{	// Add teapot (controlled by keyboard0
		::g_pObjectToRotate = new cGameObject();
		::g_pObjectToRotate->bIsUpdatedByPhysics = true;	// 
		::g_pObjectToRotate->bIsWireframe = false;
		::g_pObjectToRotate->qOrientaion 
				= glm::quat( glm::vec3(-3.14f/2.0f, 0.0f, 0.0f) );
		::g_pObjectToRotate->meshName = "Utah_Teapot.ply";
		::g_pObjectToRotate->scale = 15.0f;			// Ground is 50.0x50.0 

		//Add a Lua "brain" to this object
		::g_pObjectToRotate->CreateABrainForMe( &(::g_vec_pGOs) );

		// You would read this from a file, or whatever, maybe?
		// Note that Lua reads line-by-line, so may NEED that newline 
		//	character at the end of each line
		std::string LuaScriptToMove = 
			"r, x, y, z, vx, vy, vz = getObjectState(ObjectID) \n"	\
			"if ( r == true ) then \n"								\
			"	print( ObjectID, \":xyz=\", x, \",\", y, \", \", z ) \n"		\
			"end	\n"												\
	        " speed = 2.0   \n"
			" x = x + deltaTime * speed  \n"										\
			"setObjectState(ObjectID, x, y, z, Vx, Vy, Vz )";
		::g_pObjectToRotate->pLuaBrain->LoadScript("MoveScript", LuaScriptToMove );

		// Add the lua script to move the objects around

		vec_pGOs.push_back( ::g_pObjectToRotate );

		cGameObject* pCow = new cGameObject();
		pCow->bIsUpdatedByPhysics = true;	// 
		pCow->bIsWireframe = false;
		pCow->meshName = "cow_N.ply";
		pCow->rotSpeed.y = 0.25f;
		pCow->scale = 1.0f;
		pCow->x = -8.0f;
		::g_pObjectToRotate->vec_pChildren.push_back( pCow );

	}

	{	// Another cow
		cGameObject* pCow = new cGameObject();
		pCow->bIsUpdatedByPhysics = true;	// 
		pCow->bIsWireframe = false;
		pCow->meshName = "cow_N.ply";
		//pCow->rotSpeed.y = 0.25f;
		pCow->scale = 3.0f;
		pCow->y = 6.0f;
		pCow->CreateABrainForMe( &(::g_vec_pGOs) );
		//std::string CowLuaScriptToMove = 
		//	"r, x, y, z, vx, vy, vz = getObjectState(ObjectID) \n"	\
		//	"if ( r == true ) then \n"								\
		//	"	print( ObjectID, \":xyz=\", x, \",\", y, \", \", z ) \n"		\
		//	"end	\n"												\
		//	" y = y - 0.01  \n"										\
		//	"setObjectState(ObjectID, x, y, z, Vx, Vy, Vz )";
		std::string CowLuaScriptToMove = 
			"r, x, y, z, vx, vy, vz = getObjectState(ObjectID) \n"	\
			"if ( r == true ) then \n"								\
			"	print( ObjectID, \":xyz=\", x, \",\", y, \", \", z ) \n"		\
			"end	\n"												\
			"if ( y > 5 ) then \n"			\
			"     Vy = -1  \n"				\
			"end \n"						\
			"if ( y < -5 ) then \n"			\
			"	  Vy = 1   \n"				\
			"end \n"						\
			"setObjectState(ObjectID, x, y, z, Vx, Vy, Vz )";
		pCow->pLuaBrain->LoadScript("CowScript", CowLuaScriptToMove );
		vec_pGOs.push_back( pCow );
	}

	{// Add the "skybox" object
		::g_pSkyBoxObject = new cGameObject();
		::g_pSkyBoxObject->bIsUpdatedByPhysics = false;	// 
//::g_pSkyBoxObject->bIsWireframe = true;
//::g_pSkyBoxObject->bUseDebugColours = true;
		::g_pSkyBoxObject->solid_R = 1.0f;
		::g_pSkyBoxObject->solid_G = 1.0f;
		::g_pSkyBoxObject->solid_B = 1.0f;
		::g_pSkyBoxObject->meshName = "Isoshphere_xyz_InvertedNormals.ply";
		::g_pSkyBoxObject->scale = 10.0f;
	}

	return true;
}