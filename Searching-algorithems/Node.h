#pragma once
#include <vector>
#include "Edge.h"


using namespace std;

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int PATH = 4; // belongs to the path to target
const int GRAY = 5; // Fringe
const int BLACK = 6; // VISITED


class Node
{
public:
	Node();
	Node( Node *other);
	//Node(Node * other, int g);
	~Node();

private:
	int value;
	Node* parent;
	int line, column;
	bool visit1; // for bidirectional bfs
	bool visit2;
	int g; // distance from start - num edges
	double h; // direct distance from current node to end node
	double f;
	vector <Edge*> outgoing;


public:
	void setVisit1(bool visit);
	bool getVisit1();
	void setVisit2(bool visit);
	bool getVisit2();
	void SetParent(Node * p);
	void SetValue(int value);
	void SetLine(int line);
	void SetColumn(int col);
	void Clear();
	int GetValue();
	int GetLine();
	int GetColumn();
	Node * GetParent();
	void AddEdge2Outgoing(Edge * pe);
	vector<Edge*> GetOutgoing();

	int GetG();
	void SetG(int g);

	double Distance(Node* end);
	void SetH(Node* end);
	double GetH();

	double GetF();
	void SetF();

};

