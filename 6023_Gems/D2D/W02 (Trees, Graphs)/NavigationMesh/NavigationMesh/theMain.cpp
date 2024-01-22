#include <iostream>
#include "cMeshNav.h"

int main()
{
	cMeshNav navMesh;

	if ( navMesh.CalculateNavMeshFromPly("MeshLab_default_fractal_terrain_XYZ_only.ply") )
	{
		std::cout << "error loading nav mesh" << std::endl;
		return -1;
	}

	std::cout << "Nav mesh loaded OK" << std::endl;


	return 0;
}
