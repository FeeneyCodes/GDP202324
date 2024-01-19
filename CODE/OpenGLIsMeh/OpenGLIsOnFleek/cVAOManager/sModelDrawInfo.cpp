#include "sModelDrawInfo.h"

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;	// or NULL
	this->pIndices = 0;		// or NULL

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	this->maxExtents_XYZ = glm::vec3(0.0f);
	this->minExtents_XYZ = glm::vec3(0.0f);
	this->deltaExtents_XYZ = glm::vec3(0.0f);

//	scale = 5.0/maxExtent;		-> 5x5x5
	this->maxExtent = 0.0f;


	this->m_UniqueID = sModelDrawInfo::m_nextUniqueID;
	sModelDrawInfo::m_nextUniqueID++;

	return;
}

sModelDrawInfo::~sModelDrawInfo()
{

}

void sModelDrawInfo::FreeMemory(void)
{
	if (this->pVertices)
	{
		delete[] this->pVertices;
	}
	if (this->pIndices)
	{
		delete[] this->pIndices;
	}
	return;
}

//static 
unsigned int sModelDrawInfo::m_nextUniqueID = sModelDrawInfo::FIRST_UNIQUE_ID;


unsigned int sModelDrawInfo::getUniqueID(void)
{
	return this->m_UniqueID;
}

void sModelDrawInfo::calcExtents(void)
{
	if ( this->pVertices == NULL )
	{
		// No vertices to check
		return;
	}

	// Set the 1st vertex as max and min
	sVertex* pCurrentVert = &(this->pVertices[0]);
	
	this->minExtents_XYZ.x = pCurrentVert->x;
	this->minExtents_XYZ.y = pCurrentVert->y;
	this->minExtents_XYZ.z = pCurrentVert->z;

	this->maxExtents_XYZ.x = pCurrentVert->x;
	this->maxExtents_XYZ.y = pCurrentVert->y;
	this->maxExtents_XYZ.z = pCurrentVert->z;

	for ( unsigned int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++ )
	{
		sVertex* pCurrentVert = &(this->pVertices[vertIndex]);

		if (pCurrentVert->x < this->minExtents_XYZ.x) { this->minExtents_XYZ.x = pCurrentVert->x; }
		if (pCurrentVert->y < this->minExtents_XYZ.y) { this->minExtents_XYZ.y = pCurrentVert->y; }
		if (pCurrentVert->z < this->minExtents_XYZ.z) { this->minExtents_XYZ.z = pCurrentVert->z; }

		if (pCurrentVert->x > this->maxExtents_XYZ.x) { this->maxExtents_XYZ.x = pCurrentVert->x; }
		if (pCurrentVert->y > this->maxExtents_XYZ.y) { this->maxExtents_XYZ.y = pCurrentVert->y; }
		if (pCurrentVert->z > this->maxExtents_XYZ.z) { this->maxExtents_XYZ.z = pCurrentVert->z; }

	}//for ( unsigned int vertIndex = 0...

	// Calculate deltas...
	this->deltaExtents_XYZ.x = this->maxExtents_XYZ.x - this->minExtents_XYZ.x;
	this->deltaExtents_XYZ.y = this->maxExtents_XYZ.y - this->minExtents_XYZ.y;
	this->deltaExtents_XYZ.z = this->maxExtents_XYZ.z - this->minExtents_XYZ.z;

	// Largest extent...
	this->maxExtent = this->deltaExtents_XYZ.x;
	if (this->maxExtent < this->deltaExtents_XYZ.y) { this->maxExtent = this->deltaExtents_XYZ.y; }
	if (this->maxExtent < this->deltaExtents_XYZ.z) { this->maxExtent = this->deltaExtents_XYZ.z; }

	return;
}
