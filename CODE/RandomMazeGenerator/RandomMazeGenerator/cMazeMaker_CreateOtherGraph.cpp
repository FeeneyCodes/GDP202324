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
//				pNewRoom->id = (a * 1000) + b;
				pNewRoom->id = this->calcID(a, b);
				this->vecRooms.push_back(pNewRoom);
			}
		}
	}

	// 2. Go through these rooms and using the original 2D array, 
	//		see what rooms are connected to this one.
	for (unsigned int a = 0; a < this->maze.size(); a++)
	{
		for (unsigned int b = 0; b < this->maze[a].size(); b++)
		{
			if ( ! this->maze[a][b][0])
			{
				//mazeFile << " ";
				// Is a room
				unsigned int id_to_find = this->calcID(a, b);
				// find associated room in vector or rooms
				cMazeRoom* pTestRoom = this->pFindRoomByID(id_to_find);

				// Check each direction to see if there's a room to go to
				// check 'West' (to the left)
				if ( b > 0 )	// Edge case: colum is left most
				{
					// Room to the West or left
					if ( ! this->maze[a][b-1][0] )
					{
						// it's a room
						unsigned int room_to_left_id = this->calcID(a, b - 1);
						pTestRoom->vec_pAdjoiningRooms.push_back(this->pFindRoomByID(room_to_left_id));
					}
				}//if (b > 0)

				// Check East or right
				if ( b < (this->maze[a].size()-1) )	// Edge case - column is east most
				{
					// Room to the East or right
					if (!this->maze[a][b + 1][0])
					{
						// it's a room
						unsigned int room_to_right_id = this->calcID(a, b + 1);
						pTestRoom->vec_pAdjoiningRooms.push_back(this->pFindRoomByID(room_to_right_id));
					}
				}//if ( b < this->maze[a].size() )

				// Check North or above
				if (a > 0)	// Edge case - row is top most
				{
					// Room to the North or above
					if (!this->maze[a - 1][b][0])
					{
						// it's a room
						unsigned int room_above_id = this->calcID(a -1 , b);
						pTestRoom->vec_pAdjoiningRooms.push_back(this->pFindRoomByID(room_above_id));
					}
				}//if (a > 0)

				// Check South or below
				if (a < (this->maze.size()-1))	// Edge case - row is bottom most
				{
					// Room to the North or above
					if (!this->maze[a + 1][b][0])
					{
						// it's a room
						unsigned int room_belolw_id = this->calcID(a + 1, b);
						pTestRoom->vec_pAdjoiningRooms.push_back(this->pFindRoomByID(room_belolw_id));
					}
				}//if (a < this->maze.size())

			}
		}//for (unsigned int b...
	}//for (unsigned int a

	return;
}

unsigned int cMazeMaker::calcID(unsigned int row, unsigned int col)
{
	return row * MAZE_ROW_MULTIPLE + col;
}


cMazeRoom* cMazeMaker::pFindRoomByID(unsigned int idToFind)
{
	for ( cMazeRoom* pCurrentRoom : this->vecRooms)
	{
		if ( pCurrentRoom->id == idToFind )
		{
			return pCurrentRoom;
		}
	}
	// Did't find it
	return NULL;
}
