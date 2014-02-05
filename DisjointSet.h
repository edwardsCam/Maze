#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <iostream>
#include <vector>

using namespace std;

struct node {
	int val;
	int head;
	node * next;
	bool isHead;
	node(int i) {val = i; head = i;next = NULL;isHead = true;}
};

class DisjointSet {
public:
	DisjointSet(int s);
	void unionSets(int x, int y);
	int find(int i);
	node * getAt(int i);
	bool areConnected(int x, int y);
	bool allUnited();
	int getSize();
	string toString();
private:
	int size;
	vector<node*> forest;
};

#endif