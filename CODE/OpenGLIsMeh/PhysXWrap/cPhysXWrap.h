#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Forward declare for pointer to implementation
class cPhysXWrap_Imp;

#include <string>
#include <vector>

#include "cPhysicsObjectTypes.h"


class cPhysXWrap
{
public:
	cPhysXWrap();
	~cPhysXWrap();

	void initPhysics(bool interactive, std::string PVD_Host_Address = "127.0.0.1");
	void cleanupPhysics(bool interactive);

	void AddBox(std::string name, glm::vec3 halfExtents, glm::vec3 position, glm::vec4 EulerOrientation, float mass);
	void AddSphere(std::string name, float radius, glm::vec3 position, glm::vec4 EulerOrientation, float mass);
	void AddCapsule(std::string name, glm::vec3 position, glm::vec4 EulerOrientation, float mass);


	void update(void);

	void getSceneActors(std::vector<cPhysicsObjectTypes> &vecPhysActors );


	// HACK: Remove this later
	void HACK_ShootBall(void);

private:
	cPhysXWrap_Imp* m_pImp = nullptr;
};

