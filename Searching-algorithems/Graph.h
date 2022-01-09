#pragma once
#include <vector>
#include "Node.h"
#include "Edge.h"

using namespace std;


class Graph
{
public:
	Graph();
	~Graph();
private:
	vector <Node*> nodes;
	vector <Edge*> edges;
public:
	void AddNode(Node* pn);
	void AddEdge(Edge* pe);
	Node * GetNode(int index);
	Edge * GetEdge(int index);
	vector <Node *> GetNodes();
};

