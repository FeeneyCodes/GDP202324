#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <vector>

#include "cVAOManager/sModelDrawInfo.h"

class cSoftBodyVerlet
{
public:
	cSoftBodyVerlet();
	~cSoftBodyVerlet();

	// This is for loading the original model
	// The 2nd param is the identy matrix 
	bool CreateSoftBody(sModelDrawInfo ModelDrawInfo, glm::mat4 matInitalTransform = glm::mat4(1.0f));

	// Create random constraints within the object to 'brace' the shape
	// These are invisible, though
	void CreateRandomBracing(unsigned int numberOfBraces,
							 float minDistanceBetweenVertices);

	// This will update the draw info vertex information from the 
	//	soft body simulation state (at the current moment)
	bool UpdateDrawInfo(void);

	void VerletUpdate(double deltaTime);
	// This is for the ground, hitting something, etc.
	void ApplyCollision(double deltaTime);

	void SatisfyConstraints(void);

	struct sParticle
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 old_position = glm::vec3(0.0f);
		// Pointer back to the model vertex info
		sVertex* pModelVertex = NULL;
	};

	void cleanZeros(glm::vec3& value);


	float calcDistanceBetween(sParticle* pPartA, sParticle* pPartB);

	// These are the "rods" or "springs" between vertices
	// We get these from the mesh, BUT we might need additional
	//	constraints to make sure this stays in the desired shape
	struct sConstraint
	{
		sConstraint() {}
		sParticle* pParticleA = NULL;
		sParticle* pParticleB = NULL;
		// How far away they are supposed to be 'at rest'
		float restLength = 0.0f;

		// if false, this isnt' checked
		// Like if the constraint is 'broken'
		bool bIsActive = true;		
		// This is interesting, too
		// From: I Spent a Week Making an AI's Video Game Idea - YouTube
		// https://www.youtube.com/watch?v=PGk0rnyTa1U&ab_channel=SebastianLague
		// He's using it as to fix the end of the constraint
		bool bIsLocked = false;
	};

	// Force, like gravity or whatever
	glm::vec3 acceleration = glm::vec3(0.0f);


	std::vector< sParticle* > vec_pParticles;
	std::vector< sConstraint* > vec_pConstraints;

	sModelDrawInfo m_ModelVertexInfo;

	// Max update time is 60Hz
	const double MAX_DELTATIME = 1.0 / 60.0;

};

