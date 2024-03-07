#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

#include <float.h>		// for floating point limit


class cMesh 
{
public:
	cMesh();		// Called on creation   c'tor
	~cMesh();		// Called on deletion   d'tor

	// There's tons of these assignment operators based on various ways of assigning
	// https://cplusplus.com/articles/y8hv0pDG/
	cMesh*& operator=(const cMesh*& pMesh);
	cMesh& operator=(const cMesh& theMesh);

	//https://en.cppreference.com/w/cpp/memory/new/operator_new
	static void* operator new(size_t size);
//	static void* operator new[](std::size_t count);

	//https://en.cppreference.com/w/cpp/memory/new/operator_delete
	static void operator delete (void* ptr);
//	static void operator delete[](void* ptr);

	static void InitMemory(void);
	//	cMesh* m_pTheMeshes = new cMesh[cMesh::MAX_NUMBER_MESHES];	// Heap
	//  cMesh m_pTheMeshes[MAX_NUMBER_MESHES]						// Stack
	//
	//	memset(cMesh::m_pTheMeshes, 0, sizeof(cMesh)* cMesh::MAX_NUMBER_MESHES);

private:
	// Where our data actually is
	static cMesh* m_TheMeshes;// = nullptr;
	static bool* m_MeshInUse;// = nullptr;
	static const unsigned int MAX_NUMBER_MESHES = 10'000;
	static unsigned int m_numMeshInUse;// = 0;

//	std::vector< cMesh > vecTheMeshes;
	// Push-back 10,000 time

	// this is now many references (pointers) we have to this object
	unsigned int m_refCount = 0;

public:

//	void Update();

	// Texture info
	static const int NUM_TEXTURES = 8;
	std::string textureName[NUM_TEXTURES];		//	"\0"
	float textureRatios[NUM_TEXTURES];
	
	// HACK:
	// Here is some framebuffer object textures
	// If this is true, then we assign the colour texture from the FBO
	bool textureIsFromFBO = false;
	unsigned int FBOTextureNumber = 0;

	// 592 bytes without
	// 600 bytes WITH --> 8 bytes
	char SingleByteToMessWithAllocation = 0;

	std::string meshName;

	struct sLODInfo
	{
		// If we are within THIS distance...
		float minDistance = FLT_MAX;		// Max floating point value
		// Draw this mesh
		std::string meshName;

		sLODInfo(std::string LODmeshName, float LODminDist)
		{
			this->meshName = LODmeshName;
			this->minDistance = LODminDist;
		}
	};

	std::vector<sLODInfo> vecLODs;


	std::string friendlyName;		// "Ground"

	glm::vec3 drawPosition;

//	glm::vec3 drawOrientation;

	void setRotationFromEuler(glm::vec3 newEulerAngleXYZ)
	{
		this->m_qOrientation = glm::quat(newEulerAngleXYZ);
	}

	void adjustRoationAngleFromEuler(glm::vec3 EulerAngleXYZ_Adjust)
	{
		// To combine quaternion values, you multiply them together
		// Make a quaternion that represents that CHANGE in angle
		glm::quat qChange = glm::quat(EulerAngleXYZ_Adjust);

		// Multiply them together to get the change
		// Just like with matrix math
		this->m_qOrientation *= qChange;

// Which is exactly the same as:
//		this->m_qOrientation = this->m_qOrientation * qChange;
	}

	void Update(double deltaTime);
	bool bNeedsUpdate = true;

	glm::quat get_qOrientation(void)
	{
		return this->m_qOrientation;
	}
private:
	glm::quat m_qOrientation;
public:

	glm::vec3 drawScale;
	void setUniformDrawScale(float scale);

	// STARTOF: From: iPhysicsMeshTransformAccess interface
	virtual glm::vec3 getDrawPosition(void);
	virtual glm::vec3 getDrawOrientation(void);
	virtual void setDrawPosition(const glm::vec3& newPosition);
	virtual void setDrawOrientation(const glm::vec3& newOrientation);
	virtual void setDrawOrientation(const glm::quat& newOrientation);
	// ENDOF: iPhysicsMeshTransformAccess interface

	bool bIsVisible;

	bool bUseDebugColours;
	glm::vec4 wholeObjectDebugColourRGBA;

	bool bIsWireframe;
	bool bDoNotLight;

	std::vector<cMesh*> vec_pChildMeshes;

	//void Update(double deltaTime);

	unsigned int getUniqueID(void);




private:
	unsigned int m_UniqueID;
	static const unsigned int FIRST_UNIQUE_ID = 1000;
	static unsigned int m_nextUniqueID;

};

#endif

