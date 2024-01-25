#include <iostream>
#include "cMeshNav.h"

int main()
{
	cMeshNav navMesh;

	if ( ! navMesh.CalculateNavMeshFromPly("MeshLab_default_fractal_terrain_XYZ_only.ply") )
	{
		std::cout << "error loading nav mesh" << std::endl;
		return -1;
	}

	std::cout << "Nav mesh loaded OK" << std::endl;

	glm::vec2 enemyPosition = glm::vec2(156.6f, 67.95f);
	
	cMeshNav::sNode* pClosestNode = navMesh.pFindClosestNode(enemyPosition);


	return 0;
}
