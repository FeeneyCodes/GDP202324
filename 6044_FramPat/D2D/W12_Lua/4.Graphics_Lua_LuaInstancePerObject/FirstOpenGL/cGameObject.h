// cGameObject.h
#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

// Now each GameObject can have its own Lua "brain"
#include "cLuaBrain.h"

class cGameObject
{
public:
	cGameObject();

	// This would be called each frame, and run any Lua 
	//	scripts associated with this object
	void RunLuaScripts(float deltaTime);
	// This will load a "script" that sets a 
	//	variable for "ObjectID = X" where "X"
	//	is the value of THIS objects ID.
	// This variable can now be used inside all the Lua 
	//	scripts to refer to the object it is pointing to.
	// This is becasue we are calling the static
	//	getObjectState and setObjectState methods
	void SetLuaBrainObjectID(void);
	// Creates a lua state instance if not present
	// Points the instance to this game object
	// Calls SetLuaBrainObjectID
	// Allows access to all the other objects
	// (this is sort of "dangerous" for all the 
	//  objects to have read+write to this vector, so 
	//  you might want to use a mediator pattern instead)
	bool CreateABrainForMe(std::vector< cGameObject* >* p_vecGOs);
	cLuaBrain* pLuaBrain;

	// TODO: Get rid of this pre-rotation...
//	float pre_Rot_X, pre_Rot_Y, pre_Rot_Z;		// Euler angles 
	float x, y, z;			// Position ("translation")

//	float post_Rot_X, post_Rot_Y, post_Rot_Z;	// Euler angles 
	glm::quat qOrientaion;

	// Speed of rotation around each axis.
	glm::vec3 rotSpeed;		//(1.0f, 0.0f, 0.0f);

	float scale;		// Relative to a unit scale
	float alpha;

	// Used in Physics and in graphics
	float radius;
	float lastX, lastY, lastZ;			// ADDED
	int getID(void);					// ADDED
private:
	int m_uniqueID;
	static int m_nextID;

public:

	float Vx, Vy, Vz;
	float Ax, Ay, Az;

	int meshID;	// Which model we are drawing
	std::string meshName;		// "bunny.ply" or whatever

	bool bIsWireframe;
	float solid_R, solid_G, solid_B;		// Solid RGB
	bool bUseDebugColours;
	bool bIsVisible;

	std::string texture00Name;
	std::string texture01Name;

	bool bIsImposter;

	bool bIsUpdatedByPhysics;

	//
	std::vector< cGameObject* > vec_pChildren;

};

	////  Spherical bounding object
	//float radius;

	//// AABB 
	//float deltaX, deltaY, deltaZ;
	//float minX, minY, minZ;
	//float maxX, maxY, maxZ;

	//// AABB cube
	//float distanceFromCentre;

#endif 
