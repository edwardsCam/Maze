#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

class Pos {
public: 
	Pos (int r=0, int c=0) { row = r; col = c; facing = 0; prevRow = r; prevCol = c;};
	int row,col;
	int facing; //direction
	int prevRow, prevCol; //previous location

	bool equals(Pos & p){return row==p.row && col==p.col;}
	bool operator==(Pos & p){return row==p.row && col==p.col;}

	string toString(){
		stringstream out;
		string direction;
		switch (facing) {
		case 0:
			direction = "NORTH";
			break;
		case 1:
			direction = "EAST";
			break;
		case 2:
			direction = "SOUTH";
			break;
		case 3:
			direction = "WEST";
		}
		out << "Pos [" << row << "," << col << "] facing " << direction << endl;
		return out.str();
	}

	//rotate clockwise
	void turn() {
		facing++;
		if (facing > 3) facing = 0;
		prevRow = row;
		prevCol = col;
	}

	//move forward
	void move() {
		prevRow = row;
		prevCol = col;
		switch (facing) {
		case 0:
			row -= 2;
			break;
		case 1:
			col += 2;
			break;
		case 2:
			row += 2;
			break;
		case 3:
			col -= 2;
		}
	}

	bool justMoved() {return row != prevRow || col != prevCol;}
};