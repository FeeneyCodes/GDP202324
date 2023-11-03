#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
struct sVertex
{
	float x, y, z, w;		// vPos
	float r, g, b, a;		// vCol
	float nx, ny, nz, nw;	// vNormal
	float u, v;				// vTextureCoords		<--- NEW!!
};

struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVertex* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;	// Triangles

	glm::vec3 maxExtents_XYZ;	// bounding box maximums
	glm::vec3 minExtents_XYZ;	// bounding box minimums
	glm::vec3 deltaExtents_XYZ;	// bounding box dimensions
	float maxExtent;

	void calcExtents(void);

	// 
	unsigned int getUniqueID(void);
private:
	unsigned int m_UniqueID;
	static const unsigned int FIRST_UNIQUE_ID = 1;
	static unsigned int m_nextUniqueID;
};
