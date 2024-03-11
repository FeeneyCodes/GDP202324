#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

#include "iPhysicsMeshTransformAccess.h"
#include <float.h>		// for floating point limit





//void UpdateObject(std::string friendyName)
//{
//	cMesh* pThing = ::findObjectByFriendlyName(frientlyName);
//	pThing->Update();
//}

class cMesh : public iPhysicsMeshTransformAccess
{
public:
	cMesh();		// Called on creation   c'tor
	~cMesh();		// Called on deletion   d'tor

//	void Update();

	// Texture info
	static const int NUM_TEXTURES = 8;
	std::string textureName[NUM_TEXTURES];
	float textureRatios[NUM_TEXTURES];
	
	// HACK:
	// Here is some framebuffer object textures
	// If this is true, then we assign the colour texture from the FBO
	bool textureIsFromFBO = false;
	unsigned int FBOTextureNumber = 0;

//	pThePhysXMesh* pPhysXObjectIMatchUpWith;
//	unsigned int PhysX_UniqueID;
//
//	PxRigidActor* pOPhysicObject;

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

