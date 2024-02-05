#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

class cGameEntity
{
public:
	cGameEntity();
	std::string name;
	glm::vec3 position;

	void SetTargetToChase(glm::vec3 targetPosition);
	void StartChaseTarget(void);

	void PrintPosition(void);

	void Update(void);

	unsigned int getUniqueID(void);
private:
	static unsigned int m_NextID;
	unsigned int m_UniqueID;

	const float MAX_SPEED = 1.0f;

	bool m_IsChasing = false;
	glm::vec3 m_target = glm::vec3(0.0f);
};
