#include "cCommand_MoveTo.h"

cCommand_MoveTo::cCommand_MoveTo(cMesh* pMeshToMove,
								 glm::vec3 startXYZ,
								 glm::vec3 endXYZ, 
								 float timeToMove)
{
	this->m_startXYZ = startXYZ;
	this->m_endXYZ = endXYZ;
	this->m_TimeToMove = timeToMove;

	this->m_pTheMesh = pMeshToMove;


	this->m_rayFromStartToEnd = this->m_endXYZ - this->m_startXYZ;

	this->m_direction = glm::normalize(this->m_rayFromStartToEnd);

	float totalDistance = glm::distance(this->m_endXYZ, this->m_startXYZ);
	this->m_speed = totalDistance / timeToMove;

	this->m_velocity = this->m_direction * this->m_speed;

	this->m_elapsedTime = 0.0;
}

void cCommand_MoveTo::Update(double deltaTime)
{
	// I'm deciding that when the time is up, I'm done.

	// Update the time
	this->m_elapsedTime += deltaTime;

	glm::vec3 currentXYZ = this->m_pTheMesh->getDrawPosition();

	currentXYZ += (this->m_velocity * (float)deltaTime);

	this->m_pTheMesh->setDrawPosition(currentXYZ);

	return;
}

// Also called every frame. Returns true when done
bool cCommand_MoveTo::isDone(void)
{
	if (this->m_elapsedTime >= this->m_TimeToMove )
	{
		// Yes
		return true;
	}
	// Nope
	return false;
}

