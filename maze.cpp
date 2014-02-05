#include "DisjointSet.h"
#include "maze.h"
//From current position, return the position inc away in given dir
Pos Maze::getPos(Pos curr, Dir dir,int inc=1)
{ 
	switch (dir)
	{
	case NORTH: 
		return Pos(curr.row-inc, curr.col);
	case EAST:
		return Pos(curr.row, curr.col+inc);
	case SOUTH:
		return Pos(curr.row+inc, curr.col);
	case WEST:
		return Pos(curr.row, curr.col-inc);
	}
	assert(false);
	return Pos();
}

// return direction symbol associated with Direction d
char Maze::getSymbol(int d)
{  switch(d){
case NORTH: return '^';
case SOUTH: return 'v';
case EAST:  return '>';
case WEST:  return '<';
}
assert(false);
return static_cast<Dir>('<');
}

// Return true if maze can be solved from position curr.
// fout stores debug information
// solveMaze marks solution in maze
/*
Follows the right wall
Algorithm:
-spin around until there is a wall on your right, and nothing in front
-move forward (navigate corners appropriately)
-repeat until solution is found
*/
bool Maze::solveMaze(Pos curr,ofstream & fout) {

	//follow the right wall
	while (!curr.equals(final)) {
		if (adjacentWall(curr, EAST) && !adjacentWall(curr, NORTH)) {//wall on right, nothing in front
			cell[curr.row][curr.col] = getSymbol(curr.facing);
			curr.move();}
		else if (curr.justMoved() && !adjacentWall(curr, EAST)) { //corner
			curr.turn();
			cell[curr.row][curr.col] = getSymbol(curr.facing);
			curr.move();}
		else curr.turn();
	}
	markTrail(); // draw path
	return true;
}

//Create an initial maze as defined by rows and cols
void Maze::initializeMaze(int rows, int cols) {	
	maze_rows = rows;
	maze_cols = cols;
	roomRows = maze_rows / 2;
	roomCols = maze_cols / 2;
	numRooms = roomCols * roomRows;
	// dynamically create correct size grid
	cell = new char * [maze_rows];
	for (int r = 0; r < maze_rows; r++)
		cell[r] = new char [maze_cols];

	final = Pos(maze_rows-2,maze_cols-2);
	start = Pos(1, 1);

	for(int i =0;i<maze_rows;i++){
		for(int j=0;j<maze_cols;j++){
			if(isEven(i) && isEven(j))
				cell[i][j] = WALLJOINT; // if row is odd, put wall on the position
			else if (!isEven(i) && isEven(j))    // if column is odd , put wall on the position
				cell[i][j] = WALLUP;
			else if (isEven(i) && !isEven(j))    // if column is odd , put wall on the position
				cell[i][j] = WALLACROSS;
			else
				cell[i][j] = EMPTY; // if odd and column are even, put space on the position 
		}
	}

}

// Create a maze by randomly removing walls between segments that are NOT already connected
/*
Algorithm:
-Start at room 1
-randomly choose direction
-if legal, remove wall in that direction. Otherwise, try next direction
-increment room (only remove one wall per room)
---repeat until all rooms are connected

*/
void Maze::createRandomMaze(int rows, int cols) {
	if (isEven(rows)) rows++;
	if (isEven(cols)) cols++;
	int r1 = 1;
	int r2 = 2;
	const int QUARTER_CHANCE = 25;

	initializeMaze(rows, cols);
	DisjointSet roomSet(numRooms); //all rooms are seperate

	cout << endl << "Generating Maze..." << endl;

	while (!roomSet.allUnited()) {	
		if (r1 > roomSet.getSize()) r1 = 1;
		int dir = rand() % 4; //start with random direction
		for (int i = 0; i < 4; i++) {
			if (dir > 3) dir = 0;
			switch (dir) {
			case 0:
				r2 = r1 -roomCols;
				if (canBulldoze(r1, r2, dir, roomSet, QUARTER_CHANCE)) { //only remove wall if it isn't a border, and the two rooms aren't connected
					bulldoze(r1, NORTH);
					roomSet.unionSets(r1, r2);
					i = 4; //exit loop
				}
				break;
			case 1:
				r2 = r1 +1;
				if (canBulldoze(r1, r2, dir, roomSet, QUARTER_CHANCE)) {
					bulldoze(r1, EAST);
					roomSet.unionSets(r1, r2);
					i = 4;
				}
				break;
			case 2:
				r2 = r1 +roomCols;
				if (canBulldoze(r1, r2, dir, roomSet, QUARTER_CHANCE)) { 
					bulldoze(r1, SOUTH);
					roomSet.unionSets(r1, r2);
					i = 4;
				}
				break;
			case 3:
				r2 = r1 -1;
				if (canBulldoze(r1, r2, dir, roomSet, QUARTER_CHANCE)) { 
					bulldoze(r1, WEST);
					roomSet.unionSets(r1, r2);
					i = 4;
				}
			}
			dir++; //try next direction
		}
		r1++; //next room
	}	
}

// Create a string version of maze, preceeded by msg.
// if cleanIt: Don't print visited markings
string Maze::toString(string msg, bool cleanIt) {
	stringstream cout;
	cout <<msg << endl<< "   ";
	for (int j=0; j < maze_cols;j++)
		cout << j%10;
	cout<< endl;
	for (int i = 0; i < maze_rows;i++){
		cout << setw(2)<<i <<" ";
		for (int j=0; j < maze_cols;j++)
			if (cleanIt && cell[i][j]== VISITED)
				cout << " ";
			else
				cout << cell[i][j];
		cout << " " << setw(2) << i <<endl;
	}
	cout<<  "   ";   
	for (int j=0; j < maze_cols;j++)
		cout << j%10;
	cout<< endl; 
	return cout.str();
}

// Read the maze from the given filename.  
void Maze::readMaze(char * filename)
{  
	ifstream fin;
	fin.open(filename);
	assert(fin!=NULL);
	string str;
	fin >> maze_rows >> maze_cols;
	fin.ignore();  // Ignore end of line character
	initializeMaze(maze_rows, maze_cols);
	for(int i=0;i<maze_rows;i++)
	{
		getline(fin,str);
		cout << "STRING$" << str << "$"<< str.length() << endl;
		assert((int) str.length() >=maze_cols);
		for(int j=0;j<maze_cols;j++)
			cell[i][j] = str[j];
	}
	fin.close();
}

//draw the path from start to finish.
/*
Initially, the path contains every visited room
However, the arrows all flow into each other.
The quickest path is found by starting at the beginning and following the arrows.
*/
void Maze::markTrail() {

	//mark trail
	Pos curr = start;
	while (!curr.equals(final)){
		switch(cell[curr.row][curr.col]) {
		case '^':
			curr.facing = 0;
			cell[curr.row][curr.col] = 'n';
			break;
		case '>':
			curr.facing = 1;
			cell[curr.row][curr.col] = 'e';
			break;
		case 'v':
			curr.facing = 2;
			cell[curr.row][curr.col] = 's';
			break;
		case '<':
			curr.facing = 3;
			cell[curr.row][curr.col] = 'w';}	
		curr.move();		
	}

	//clear all rooms not on the trail
	for (int i = 1; i < maze_rows; i+=2) 
		for (int j = 1; j < maze_cols; j+=2) 
			if (cell[i][j] == '^' || cell[i][j] == '>' || cell[i][j] == 'v' || cell[i][j] == '<') 
				cell[i][j] = ' ';

	curr = start;
	while (!curr.equals(final)){
		switch(cell[curr.row][curr.col]) {
		case 'n':
			curr.facing = 0;
			cell[curr.row][curr.col] = '^';
			break;
		case 'e':
			curr.facing = 1;
			cell[curr.row][curr.col] = '>';
			break;
		case 's':
			curr.facing = 2;
			cell[curr.row][curr.col] = 'v';
			break;
		case 'w':
			curr.facing = 3;
			cell[curr.row][curr.col] = '<';}	
		curr.move();		
	}
	cell[start.row][start.col] = 'o';
	cell[final.row][final.col] = 'x';
}

/*
Only remove a wall if...
a. random probability allows
b. wall is not a border
c. adjoining rooms are not already connected
*/
bool Maze::canBulldoze(int r1, int r2, int x, DisjointSet roomSet, int chance)
{return rand()%100<chance && !isBorder(r1, x) && !roomSet.areConnected(r1, r2);}

//given a room number, remove the wall in the specified direction
//int room: the room number
//Dir d: direction from room to delete the wall
void Maze::bulldoze(int r, Dir d) {
	int i = ((r -1) /roomCols) *2 +1;
	int j = r%roomCols *2 -1;
	int maxJ = roomCols *2 -1;
	switch (d){
	case 0:
		if (r % roomCols == 0) j = maxJ;
		i--; break;
	case 1:
		if (r % roomCols == 0) j = ++maxJ;
		else j++; break;
	case 2:
		if (r % roomCols == 0) j = maxJ;
		i++; break;
	case 3:
		if (r % roomCols == 0) j = --maxJ;
		else j--;}
	cell[i][j] = ' ';
}

//given a pos, return whether it's adjacent to a wall in any direction (relative to where p is facing)
//Pos p: the current position
//Dir d: the direction to check for a wall (d is relative to p's orientation)
bool Maze::adjacentWall(Pos p, Dir d) {
	switch (d) {
	case 0:
		switch (p.facing) {
		case NORTH:
			return cell[p.row-1][p.col] == '-';
		case EAST:
			return cell[p.row][p.col+1] == '|';
		case SOUTH:
			return cell[p.row+1][p.col] == '-';
		case WEST:
			return cell[p.row][p.col-1] == '|';}
		break;
	case 1:
		switch (p.facing) {
		case NORTH:
			return cell[p.row][p.col+1] == '|';
		case EAST:
			return cell[p.row+1][p.col] == '-';
		case SOUTH:
			return cell[p.row][p.col-1] == '|';
		case WEST:
			return cell[p.row-1][p.col] == '-';}
		break;
	case 2:
		switch (p.facing) {
		case NORTH:
			return cell[p.row+1][p.col] == '-';
		case EAST:
			return cell[p.row][p.col-1] == '|';
		case SOUTH:
			return cell[p.row-1][p.col] == '-';
		case WEST:
			return cell[p.row][p.col+1] == '|';}
		break;
	case 3:
		switch (p.facing) {
		case NORTH:
			return cell[p.row][p.col-1] == '|';
		case EAST:
			return cell[p.row-1][p.col] == '-';
		case SOUTH:
			return cell[p.row][p.col+1] == '|';
		case WEST:
			return cell[p.row+1][p.col] == '-';}
	default:return false;}
}

//given a room, determine if the wall in direction d is a border
bool Maze::isBorder(int r, int d) {
	switch (d){
	case 0:
		return r <= roomCols;break;
	case 1:
		return r % roomCols == 0;break;
	case 2:
		return r > (roomRows - 1) * roomCols;break;
	case 3:
		return r % roomCols == 1;
	default:return true;}
}