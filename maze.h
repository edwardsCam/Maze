
#include "Pos.h"
#include "DisjointSet.h"
#include <string>
#include <assert.h>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

enum{WALLUP='|', WALLACROSS= '-', WALLJOINT = '+', EMPTY=' ',VISITED = '@'};
enum Dir {NORTH, EAST, SOUTH, WEST};

class Maze {
private:
	Pos start;   // Location where path search begins
	Pos final;   // Location where path search ends
	int maze_rows, maze_cols;  //Size of cell
	int roomRows, roomCols; //Number of rooms
	int numRooms; //Number of rooms
	char **cell;               // Grid of characters (including walls) of maze
	bool solveMaze(Pos curr,ofstream & fout);
	bool isEven(int val){ return ((val%2)==0);}
	void initializeMaze(int rows, int cols);
	char getSymbol(int d);
	Pos getPos(Pos curr, Dir dir,int inc);
	void mark(Pos p,char c) {cell[p.row][p.col] = c;};
	void markTrail();
	bool canBulldoze(int r1, int r2, int x, DisjointSet roomSet, int chance);
	void bulldoze(int r1, Dir d);
	bool adjacentWall(Pos p, Dir d);
	bool isBorder(int r, int d);	
public:
	Maze(){cell = NULL;};
	void readMaze(char *);
	string toString(string msg="", bool cleanIt=false);
	void createRandomMaze(int rows, int cols);
	bool solveMaze(ofstream & fout){return solveMaze(start,fout);}
};