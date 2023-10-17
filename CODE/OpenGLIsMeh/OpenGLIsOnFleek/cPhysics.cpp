#include "cPhysics.h"


cPhysics::cPhysics()
{
	
}

cPhysics::~cPhysics()
{
}

void cPhysics::setVAOManager(cVAOManager* pTheMeshManager)
{
	this->m_pMeshManager = pTheMeshManager;
	return;
}


void cPhysics::AddShape(sPhsyicsProperties* pNewShape)
{
	this->m_vec_pPhysicalProps.push_back(pNewShape);

	// Does this have an associated mesh?
	if (pNewShape->pTheAssociatedMesh)
	{
		// Update the drawing positions from the physics information
		pNewShape->pTheAssociatedMesh->setDrawPosition(pNewShape->position);
		//pNewShape->pTheAssociatedMesh->setDrawOrientation(pNewShape->orientation);
		pNewShape->pTheAssociatedMesh->setDrawOrientation(pNewShape->get_qOrientation());
	}

	return;
}

sPhsyicsProperties* cPhysics::findShapeByUniqueID(unsigned int uniqueIDtoFind)
{
	for (sPhsyicsProperties* pCurrentShape : this->m_vec_pPhysicalProps)
	{
		if ( pCurrentShape->getUniqueID() == uniqueIDtoFind )
		{
			// Found it
			return pCurrentShape;
		}
	}
	// Didn't find it
	return NULL;
}

sPhsyicsProperties* cPhysics::findShapeByFriendlyName(std::string friendlyNameToFind)
{
	for (sPhsyicsProperties* pCurrentShape : this->m_vec_pPhysicalProps)
	{
		if (pCurrentShape->friendlyName == friendlyNameToFind)
		{
			// Found it
			return pCurrentShape;
		}
	}
	// Didn't find it
	return NULL;
}

// Vector is empty if none found
std::vector<sPhsyicsProperties*> cPhysics::findShapesByType(sPhsyicsProperties::eShape shapeType)
{
	std::vector<sPhsyicsProperties*> vecShapesFound;
	for (sPhsyicsProperties* pCurrentShape : this->m_vec_pPhysicalProps)
	{
		if (pCurrentShape->shapeType == shapeType)
		{
			vecShapesFound.push_back(pCurrentShape);
		}
	}
	return vecShapesFound;
}

// Returns false if none found
bool cPhysics::findShapesByType(sPhsyicsProperties::eShape shapeType, std::vector<sPhsyicsProperties*>& vecShapes)
{
	for (sPhsyicsProperties* pCurrentShape : this->m_vec_pPhysicalProps)
	{
		if (pCurrentShape->shapeType == shapeType)
		{
			vecShapes.push_back(pCurrentShape);
		}
	}
	return ( ! vecShapes.empty() );
}



