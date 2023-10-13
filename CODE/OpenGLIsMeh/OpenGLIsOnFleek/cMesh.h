#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "iPhysicsMeshTransformAccess.h"

class cMesh : public iPhysicsMeshTransformAccess
{
public:
	cMesh();		// Called on creation   c'tor
	~cMesh();		// Called on deletion   d'tor

	std::string meshName;

	std::string friendlyName;		// "Ground"

	glm::vec3 drawPosition;
	glm::vec3 drawOrientation;
	glm::vec3 drawScale;
	void setUniformDrawScale(float scale);

	// STARTOF: From: iPhysicsMeshTransformAccess interface
	virtual glm::vec3 getDrawPosition(void);
	virtual glm::vec3 getDrawOrientation(void);
	virtual void setDrawPosition(const glm::vec3& newPosition);
	virtual void setDrawOrientation(const glm::vec3& newOrientation);
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

