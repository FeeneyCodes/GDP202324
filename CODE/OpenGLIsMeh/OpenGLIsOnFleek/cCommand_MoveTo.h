#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "cMesh.h"

class iCommand
{
public:
	virtual ~iCommand() {};

	// Some common initialization data being passed
	// (common to all the commands)
	// (You could use a name-value par - like Quake 2 - or XML, JSON, whatever)
	virtual void Init(cMesh* pMesh, std::vector<glm::vec3> vecofVec3s) = 0;

	// Call every frame
	virtual void Update(double deltaTime) = 0;
	// Also called every frame. Returns true when done
	virtual bool isDone(void) = 0;
};

class cCommand_MoveTo
{
public:
	cCommand_MoveTo(cMesh* pMeshToMove, 
					glm::vec3 startXYZ, glm::vec3 endXYZ, 
					float timeToMove);

	// Call every frame
	void Update(double deltaTime);
	// Also called every frame. Returns true when done
	bool isDone(void);

private:
	// Can't call default constructor
	cCommand_MoveTo() {};

	glm::vec3 m_startXYZ, m_endXYZ;
	float m_TimeToMove;
	cMesh* m_pTheMesh;

	// These are calculated
	glm::vec3 m_rayFromStartToEnd;
	glm::vec3 m_direction;
	glm::vec3 m_velocity;
	float m_speed;

	double m_elapsedTime;
};
