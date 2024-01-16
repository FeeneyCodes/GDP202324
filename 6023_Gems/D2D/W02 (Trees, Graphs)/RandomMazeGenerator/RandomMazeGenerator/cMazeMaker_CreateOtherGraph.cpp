#include "cMazeMaker.h"

void cMazeMaker::CreateOtherGraph(void)
{
	//std::vector< cMazeRoom* > vecRooms;

	// 1. Add only the rooms (not walls) to this vector
	for (unsigned int a = 0; a < this->maze.size(); a++)
	{
		for (unsigned int b = 0; b < this->maze[a].size(); b++)
		{
			if (this->maze[a][b][0])
			{
				//mazeFile << "X";
				// Do nothing
			}
			else
			{
				//mazeFile << " ";

				cMazeRoom* pNewRoom = new cMazeRoom();
				// Room ID is "4" digits
				// a = 5, b = 11  ---> 511
				pNewRoom->id = (a * 100) + b;
				vecRooms.push_back(pNewRoom);
			}
		}

	}

	// 2. Go through these rooms and using the original 2D array, 
	//		see what rooms are connected to this one.


	return;
}