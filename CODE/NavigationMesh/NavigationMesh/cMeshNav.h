#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class cMeshNav
{
public:
	cMeshNav();

	// Load a ply file as a navigation mesh, 
	//	calculating the "cost" based on difference in height
	// Assumes: 
	//  - only XYZ format for PLY file
	//  - Y is height
	//  - X & Z are positive
	bool CalculateNavMeshFromPly(std::string plyFileName);

	// What do we need here? 
	struct sNode
	{
		glm::vec3 position = glm::vec3(0.0f);
		// This lists:
		// - Possible nodes we can move to from this node
		// - the 'cost' to move to that node
		struct sNodeCostPair
		{
			sNodeCostPair() {};
			sNode* pNode = NULL;
			float cost = 0;
			// Helpful constructor
			sNodeCostPair(sNode* pNode_, float cost_)
			{
				this->pNode = pNode_;
				this->cost = cost_;
			};
		};
		std::vector< sNodeCostPair > vec_pAdjacentNodes;
	// Or...
//		std::vector< std::pair<sNode*, float /*cost*/ > > vec_pAdjacentNodes;
	};

	// These are effectively the vertices from the original model
	std::vector< sNode* > vec_pNodes;

	// Cost to go from node start to node end
	float calcCost(sNode* pStart, sNode* pEnd);

	// returns closest vertex node to this point
	// i.e. We place our character at XZ and see what nav node is closest
	sNode* pFindClosestNode(glm::vec2 XZ);


};
