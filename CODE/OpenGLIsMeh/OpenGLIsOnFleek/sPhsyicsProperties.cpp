#include "sPhsyicsProperties.h"

sPhsyicsProperties::sPhsyicsProperties()
{
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
	this->inverse_mass = 1.0f;	// What should this be??
	//
	this->pTheAssociatedMesh = NULL;
	//
	this->shapeType = UNKNOWN_OR_UNDEFINED;
	//

	this->m_UniqueID = sPhsyicsProperties::m_nextUniqueID;
	sPhsyicsProperties::m_nextUniqueID++;

}

std::string sPhsyicsProperties::getShapeTypeAsString(void)
{
	switch (this->shapeType)
	{
	case SPHERE:
		return "SPHERE";
		break;
	case PLANE:
		return "PLANE";
		break;
	case TRIANGLE:
		return "TRIANGLE";
		break;
	case AABB:
		return "AABB";
		break;
	case CAPSULE:
		return "CAPSULE";
		break;
	case MESH_OF_TRIANGLES_INDIRECT:
		return "MESH_OF_TRIANGLES_INDIRECT";
		break;
	case MESH_OF_TRIANGLES_LOCAL_VERTICES:
		return "MESH_OF_TRIANGLES_LOCAL_VERTICES";
		break;
	case UNKNOWN_OR_UNDEFINED:
		return "UNKNOWN_OR_UNDEFINED";
		break;
	}

	// ERROR: UNKNOWN_OR_UNDEFINED
	return "ERROR: Invalid shape type";
}


unsigned int sPhsyicsProperties::getUniqueID(void)
{
	return this->m_UniqueID;
}

//static 
unsigned int sPhsyicsProperties::m_nextUniqueID = sPhsyicsProperties::FIRST_UNIQUE_ID;


void sPhsyicsProperties::setShape(sSphere* pSphereProps)
{
	this->pShape = (void*)pSphereProps;
	return;
}

void sPhsyicsProperties::setShape(sPlane* pPlaneProps)
{
	this->pShape = (void*)pPlaneProps;
	return;
}

void sPhsyicsProperties::setShape(sTriangle* pTriangleProps)
{
	this->pShape = (void*)pTriangleProps;
	return;
}

void sPhsyicsProperties::setShape(sCapsule* pCapsuleProps)
{
	this->pShape = (void*)pCapsuleProps;
	return;
}

void sPhsyicsProperties::setShape(sMeshOfTriangles_Indirect* pTriangleMeshProps)
{
	this->pShape = (void*)pTriangleMeshProps;
	return;
}

void sPhsyicsProperties::setShape(sMeshOfTriangles_LocalVertices* pTriangleMeshProps)
{
	this->pShape = (void*)pTriangleMeshProps;
	return;
}



