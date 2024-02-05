#include "cGameEntity.h"
#include <iostream>

cGameEntity::cGameEntity()
{
	this->m_UniqueID = cGameEntity::m_NextID++;
}

unsigned int cGameEntity::getUniqueID(void)
{
	return this->m_UniqueID;
}

//static
unsigned int cGameEntity::m_NextID = 100;

void cGameEntity::Update(void)
{
	// TODO: Something...
	// Are we chasing something?
	if ( this->m_IsChasing )
	{
		// How far away are we?
		glm::vec3 vecToTarget = this->m_target - this->position;

		glm::vec3 moveStepThisFrame = glm::normalize(vecToTarget) * MAX_SPEED;

		this->position += moveStepThisFrame;
	}

	return;
}

void cGameEntity::SetTargetToChase(glm::vec3 targetPosition)
{
	this->m_target = targetPosition;
	return;
}

void cGameEntity::StartChaseTarget(void)
{
	this->m_IsChasing = true;
}

void cGameEntity::PrintPosition(void)
{
	std::cout << "Entity[" << this->getUniqueID() << "] "
		<< " is at "
		<< this->position.x << ", "
		<< this->position.y << std::endl;
	return;
}

