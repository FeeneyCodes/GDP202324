#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "iPhysicsMeshTransformAccess.h"
#include <string>
#include <limits.h>
#include <vector>


// This is where we are going to head...
struct sPhsyicsProperties
{
	sPhsyicsProperties();

	// How the physics item knows what type of object this is
	enum eShape
	{
		UNKNOWN_OR_UNDEFINED,
		SPHERE,
		PLANE,
		TRIANGLE,
		AABB,
		CAPSULE,
		MESH_OF_TRIANGLES_INDIRECT,			// By meshID (vertices stored elsewhere)
		MESH_OF_TRIANGLES_LOCAL_VERTICES	// Vertices stored in local structure
	};

	// For some shapes, like the AABB, capsule, etc.
	//	there might be an offset. 
	// 
	// STARTOF: Shape type structures
#pragma region Shape_Type_Structures
	struct sSphere
	{
		sSphere(float radius)
		{
			this->radius = radius;
		}

		float radius = 0.0f;
	};

	struct sPlane
	{
		sPlane(glm::vec3 normal)
		{
			this->normalToSurface = normal;
		}

		glm::vec3 normalToSurface;
	};

	struct sTriangle
	{
		static const unsigned int NUMBER_OF_TRIANGLE_VERTICES = 3;
		sTriangle()
		{
			this->vertices[0] = glm::vec3(0.0f);
			this->vertices[1] = glm::vec3(0.0f);
			this->vertices[2] = glm::vec3(0.0f);
		}
		sTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
		{
			this->vertices[0] = v0;
			this->vertices[1] = v1;
			this->vertices[2] = v2;
			this->calcNormal();
		}
		sTriangle(glm::vec3 vertices[NUMBER_OF_TRIANGLE_VERTICES])
		{
			this->vertices[0] = vertices[0];
			this->vertices[1] = vertices[1];
			this->vertices[2] = vertices[2];
			this->calcNormal();
		}
		glm::vec3 vertices[NUMBER_OF_TRIANGLE_VERTICES];

		void calcNormal(void)
		{
			glm::vec3 v0_1 = this->vertices[1] - this->vertices[0];
			glm::vec3 v0_2 = this->vertices[2] - this->vertices[1];
			this->normal = glm::normalize( glm::cross(v0_1, v0_2) );
			return;
		}
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec3 getCentre(void)
		{
			return ((this->vertices[0] + this->vertices[1] + this->vertices[2]) / (float)NUMBER_OF_TRIANGLE_VERTICES);
		}
	};

	struct sAABB
	{
		sAABB(glm::vec3 minXYZ, glm::vec3 maxXYZ)
		{
			this->minXYZ = minXYZ;
			this->maxXYZ = maxXYZ;
		}

		// The half widths is like the radius from the centre, but for each axes.
		sAABB(float halfwidthX, float halfwidthY, float halfwidthZ)
		{
			this->minXYZ.x = -halfwidthX;
			this->minXYZ.y = -halfwidthY;
			this->minXYZ.z = -halfwidthZ;

			this->maxXYZ.x = halfwidthX;
			this->maxXYZ.y = halfwidthY;
			this->maxXYZ.z = halfwidthZ;
		}

		glm::vec3 minXYZ = glm::vec3(0.0f);
		glm::vec3 maxXYZ = glm::vec3(0.0f);
	};

	struct sCapsule
	{
		sCapsule(float halfLength, float radius)
		{
			this->halfLength = halfLength;
			this->radius = radius;
		}

		float halfLength;
		float radius;
	};

	// This corresponds to a mesh that's been loaded by something 
	//	that might use sModelDrawInfo, which has all the vertex info
	//	along with the name and uniqueID. The VAO Manager for example.
	struct sMeshOfTriangles_Indirect
	{
		sMeshOfTriangles_Indirect(std::string meshName)
		{
			this->meshName = meshName;
		}
		sMeshOfTriangles_Indirect(unsigned int meshUniqueID)
		{
			this->meshUniqueID = meshUniqueID;
		}

		//	These are used to look up mesh information:
		std::string meshName;
		// INT_MAX is an invalid ID
		unsigned int meshUniqueID = INT_MAX;
	};

	// Here the vertices are stored "locally"
	struct sMeshOfTriangles_LocalVertices
	{
		// This triangle stores the vertices and normals here
		std::vector< sTriangle > vecTriangles;
	};

#pragma endregion Shape_Type_Structures
	// ENDOF: Shape type structures

	void setShape(sSphere* pSphereProps);
	void setShape(sPlane* pPlaneProps);
	void setShape(sTriangle* pTriangleProps);
	void setShape(sCapsule* pCapsuleProps);
	void setShape(sMeshOfTriangles_Indirect* pTriangleMeshProps);
	void setShape(sMeshOfTriangles_LocalVertices* pTriangleMeshProps);

	eShape shapeType;
	void* pShape = NULL;

	std::string getShapeTypeAsString(void);

	
	std::string friendlyName;

	glm::vec3 position = glm::vec3(0.0f);
	// Save the old position
	glm::vec3 oldPosition = glm::vec3(0.0f);

//	glm::vec3 orientation = glm::vec3(0.0f);
	void setRotationFromEuler(glm::vec3 newEulerAngleXYZ)
	{
		this->m_qOrientation = glm::quat(newEulerAngleXYZ);
	}

	void adjustRoationAngleFromEuler(glm::vec3 EulerAngleXYZ_Adjust)
	{
		glm::quat qChange = glm::quat(EulerAngleXYZ_Adjust);
		this->m_qOrientation *= qChange;
	}

	glm::quat get_qOrientation(void)
	{
		return this->m_qOrientation;
	}
private:
	glm::quat m_qOrientation;
public:

	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);


	// Since division is "slow" and we are dividing my mass, 
	// Could make this immovable by making this 0 (infinite mass)
	float inverse_mass = 0.0f;	//	float mass;		

//	cMesh* pTheAssociatedMesh;
	iPhysicsMeshTransformAccess* pTheAssociatedMesh;

	// Rotational
//	glm::vec3 momentOfWhatever;
//	glm::vec3 rotationalSpeed;

	unsigned int getUniqueID(void);
private:
	unsigned int m_UniqueID;
	static const unsigned int FIRST_UNIQUE_ID = 1;
	static unsigned int m_nextUniqueID;
};
