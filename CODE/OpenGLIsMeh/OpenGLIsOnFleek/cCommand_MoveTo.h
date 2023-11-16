#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "cMesh.h"

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
