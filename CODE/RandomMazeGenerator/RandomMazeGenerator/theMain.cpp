
// *********************************************************
// Everything in a single class thing that Feeney "wrote"...
//
#include "cMazeMaker.h"


// *********************************************************
// 
// Function signatures form Jaden Peterson's original Maze Generator:
//
#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

extern int maze_size[2];
extern int start_axis;
extern int start_side;
extern std::vector< std::vector< int > > dfs_path;
extern std::vector< std::vector< std::vector< bool > > > maze;
bool randomMove(bool first_move);
bool validInteger(char* cstr);
void generateMaze();
void initializeMaze();
void parseArgs(int argc, char* argv[]);
void printMaze();
void randomPoint(bool part);




int main(int argc, char* argv[]) 
{
	//srand(time(NULL));


// Using Feeney's "put Jaden Peterson's thing into a single class" version

	cMazeMaker theMMBig;
//	theMMBig.GenerateMaze(1000, 1000);
//	theMMBig.PrintMazeToFile("sexyMaze_1000x1000.txt");


	cMazeMaker theMM;
	theMM.GenerateMaze(20, 20);
//	theMM.PrintMaze();
	theMM.PrintMazeToFile("sexyMaze_20x20.txt");

//	theMM.CopyToCharArray();

	theMM.CreateOtherGraph();

	std::cout << std::endl;


	std::cout << std::endl;
	std::cout << "Jaden Peterson's original code output:" << std::endl;
	std::cout << std::endl;

// Using Jaden Peterson's original code:
// 
//	parseArgs(argc, argv);

	maze_size[0] = 20;
	maze_size[1] = 20;

	// The width and height must be greater than or equal to 5 or it won't work
	// The width and height must be odd or else we will have extra walls
	for (int a = 0; a < 2; a++) 
	{
		if (maze_size[a] < 5) 
		{
			maze_size[a] = 5;
		}
		else if (maze_size[a] % 2 == 0) 
		{
			maze_size[a]--;
		}
	}

	initializeMaze();
	randomPoint(false);
	randomPoint(true);
	generateMaze();

	printMaze();

	return 0;
}
