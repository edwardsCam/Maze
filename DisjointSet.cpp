#include "DisjointSet.h"
#include <sstream>

DisjointSet::DisjointSet(int s) {
	size = s;
	for (int i = 0; i <= size; i++)  
		if (i > 0) {
			node * t = new node(i);
			forest.push_back(t);
		} else forest.push_back(NULL);
}

void DisjointSet::unionSets(int x, int y) {
	int headX = find(x);
	int headY = find(y);

	if (headX == headY) return; //already unioned

	//update head for all children of y
	node * curr = forest.at(headY);
	while (curr != NULL) {
		curr->head = headX;
		curr->isHead = false;
		curr = curr->next;
	}

	//traverse to last child and add y
	curr = forest.at(headX);
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = forest.at(headY);
}

string DisjointSet::toString() {
	stringstream ss;
	ss << "Disjoint Set, from 1 to " << size << endl << endl;
	for (int i = 1; i <= size; i++) {
		if (forest.at(i)->isHead) {
			node * curr = forest.at(i);
			while (curr != NULL) {
				ss << curr->val;
				if (curr->next != NULL)
					ss << " - ";
				curr = curr->next;
			}
			ss << endl;
		}
	}
	ss << endl;
	return ss.str();
}

//check if two values are unioned
bool DisjointSet::areConnected(int x, int y)
{return forest.at(x)->head == forest.at(y)->head;}

//return false if more than one head. If all values are unioned, return true
bool DisjointSet::allUnited() {
	int ct = 0;
	for (int i = 1; i <= size; i++) {
		ct += forest.at(i)->isHead;
		if (ct > 1) return false;}
	return true;
}

//return the head
int DisjointSet::find(int i) {return forest.at(i)->head;}

int DisjointSet::getSize() {return size;}

node * DisjointSet::getAt(int i) {return forest.at(i);}