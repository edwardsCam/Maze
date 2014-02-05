#include "maze.h"
#include <windows.h>

void pause()
{	cout<<endl<<"press enter to see the result of the created Maze problem";
cin.get();
}

int main(){  
	srand(time(NULL)); //set random seed!!!
	ofstream fout;
	fout.open("mazeOut.txt");
	/////////////////////////////////////////////////////////////
	// Demonstraion of disjoint set                            //
	/////////////////////////////////////////////////////////////
	fout<<"****  Disjoint problem  **** "<<endl;

	DisjointSet mySet(10); //create disjoint set

	mySet.unionSets(1,6);
	mySet.unionSets(6,7);
	mySet.unionSets(2,3);

	fout<< "3 is a member of =" << mySet.find(3) << endl;
	fout<< "2 is a member of =" << mySet.find(2) << endl;
	mySet.unionSets(4,5);
	mySet.unionSets(6,4);
	if(mySet.find(6)==mySet.find(4)) fout<<"6 and 4 are in same class"<<endl;
	if(mySet.find(7)==mySet.find(4)) fout<<"7 and 4 are in same class"<<endl;
	cout << mySet.toString();

	////////////////////////////////////////////////////////////
	// Demonstraion of maze solve problem                     //
	////////////////////////////////////////////////////////////
	pause();

	system("cls");  // Clears the screen

	Maze maze1,maze2;

	maze1.readMaze("maze1.txt");
	cout << maze1.toString("\n**Original Maze from maze1.txt**");
	fout << maze1.toString("\n**Original Maze from maze1.txt**");
	maze1.solveMaze(fout);
	cout << maze1.toString("\n**Solved Maze**",true);
	fout << maze1.toString("\n**Solved Maze**",true);

	/////////////////////////////////////////////////////////////
	// Demonstraion of creation maze                           //
	/////////////////////////////////////////////////////////////
	pause();

	maze2.createRandomMaze(70, 70);
	fout << maze2.toString("\n**MY Created Maze**");
	cout << maze2.toString("\n**MY Created Maze**");
	maze2.solveMaze(fout);
	fout << maze2.toString("\n**Solved Maze**",false);
	cout << maze2.toString("\n**Solved Maze**", false);
	fout.close();
	cin.get();

	return 0;
}