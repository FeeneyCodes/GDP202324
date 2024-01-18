#ifndef _cMazeMaker_HG_
#define _cMazeMaker_HG_

// This code is taken from Jaden Peterson's posting on codereview:
// https://codereview.stackexchange.com/questions/135443/c-maze-generator

// I just put it into a class for you. That's it. He did all the real work!

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>


class cMazeRoom		// aka "node"
{
public:
	unsigned int id;  // x & y location, if needed
	cMazeRoom* pAdjoiningRooms[4];
};


#include <vector>

class cMazeMaker
{
public:
	cMazeMaker();
	bool GenerateMaze(unsigned int width, unsigned int height);
	void PrintMaze(void);
	void PrintMazeToFile(std::string filename);

	std::vector< std::vector< std::vector< bool > > > maze;

	// HACK: compiler 'stack' based array
	void CopyToCharArray(void);
	char TheMaze[20][20] = { 0 };

	// COMPLETELY DIFFERENT GRAPH OF THIS MAZE
	std::vector< cMazeRoom* > vecRooms;
	void CreateOtherGraph(void);

private:
	int m_maze_size[2];

	int m_start_axis;
	int m_start_side;	

	static const unsigned int UP = 0;
	static const unsigned int DOWN = 1;
	static const unsigned int LEFT = 2;
	static const unsigned int RIGHT = 3;

	std::vector< std::vector< int > > m_dfs_path;

//	int m_dfs_path_[20][20];


	bool m_randomMove(bool first_move); 
	bool m_validInteger(char* cstr);
	void m_initializeMaze(void);
	void m_randomPoint(bool part);

};

#endif