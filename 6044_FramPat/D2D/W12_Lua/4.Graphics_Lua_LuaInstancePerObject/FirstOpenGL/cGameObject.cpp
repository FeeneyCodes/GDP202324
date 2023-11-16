#include "cGameObject.h"

#include <sstream>

int cGameObject::getID(void)
{
	return this->m_uniqueID;
}
	
//static 
int cGameObject::m_nextID = 1;

cGameObject::cGameObject()
{
	this->m_uniqueID = cGameObject::m_nextID;
	cGameObject::m_nextID++;

	// C'tor to init all values to 0.0f;
	this->x = this->y = this->z = 0.0f;
	this->Vx = this->Vy = this->Vz = 0.0f;
	this->Ax = this->Ay = this->Az = 0.0f;

//	this->pre_Rot_X = this->pre_Rot_Y = this->pre_Rot_Z = 0.0f;
	//this->post_Rot_X = this->post_Rot_Y = this->post_Rot_Z = 0.0f;
	this->scale = 1.0f;

	this->bIsWireframe = false;	// Solid 
	this->solid_B = 0.0f; 
	this->solid_G = 0.0f; 
	this->solid_R = 1.0f;	// White (if solid)

	this->bIsVisible = true;
	this->bUseDebugColours = false;

	this->alpha = 1.0f;

	this->radius = 0.0f;

	this->meshID = 0;

	this->bIsImposter = false;

	// Assume they are updated by the physics "sub-system"
	this->bIsUpdatedByPhysics = false;

	this->pLuaBrain = nullptr;	// or you could set to 0 or NULL

	return;
}

void cGameObject::RunLuaScripts(float deltaTime)
{
	// Do I have a "brain" (a Lua instance)?
	if ( this->pLuaBrain )
	{	// Yes, so run any scripts
		this->pLuaBrain->Update(deltaTime);
	}
	return;
}

// This will load a "script" that sets a 
//	variable for "ObjectID = X" where "X"
//	is the value of THIS objects ID.
// This variable can now be used inside all the Lua 
//	scripts to refer to the object it is pointing to.
// This is becasue we are calling the static
//	getObjectState and setObjectState methods
void cGameObject::SetLuaBrainObjectID(void)
{
	// Do I have a "brain" (a Lua instance)?
	if ( this->pLuaBrain )
	{
		std::stringstream ssLuaScript;
		ssLuaScript << "ObjectID = " << this->getID();
		this->pLuaBrain->RunScriptImmediately( ssLuaScript.str() );
	}

	return;
}

// Creates a lua state instance if not present
// Points the instance to this game object
// Calls SetLuaBrainObjectID
bool cGameObject::CreateABrainForMe(std::vector< cGameObject* >* p_vecGOs)
{
	// Have a brain already?
	if ( this->pLuaBrain )
	{	// Yes. Well, no one needs two brains!
		return false;
	}

	// Create a lua "brain"
	this->pLuaBrain = new cLuaBrain();	
	// Set ObjectID of "this" object
	this->SetLuaBrainObjectID();
	// Tell Lua about all the objects in the world
	this->pLuaBrain->SetObjectVector(p_vecGOs);

	return true;
}
