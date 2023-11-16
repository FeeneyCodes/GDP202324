#include "cGameObject.h"

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
	return;
}
