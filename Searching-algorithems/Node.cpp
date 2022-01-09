#include "Node.h"
#include <stdio.h>



Node::Node()
{
	value = SPACE;
	parent = nullptr;
	line = 0;
	column = 0;
	visit1 = false;
	visit2 = false;
	g = 0;
	h = 0;
	f = 0;
}


Node::Node( Node *other)
{
	value = other->GetValue();
	line = other->GetLine();
	column = other->GetColumn();
	visit1 = other->visit1;
	visit2 = other->visit2;
	g = other->g;
	if (other->GetParent() == nullptr)
		parent = nullptr;
	else
		parent = new Node(other->GetParent());

}
/*
Node::Node(Node *other, double g)
{
	this->g = g;
	value = other->GetValue();
	line = other->GetLine();
	column = other->GetColumn();
	visit1 = other->visit1;
	visit2 = other->visit2;
	if (other->GetParent() == nullptr)
		parent = nullptr;
	else
		parent = new Node(other->GetParent());

}
*/

Node::~Node()
{
}

void Node::Clear() {
	value = SPACE;
	parent = nullptr;
	visit1 = false;
	visit2 = false;
	g = 0;
	h = 0;
	f = 0;
}

void Node::setVisit1(bool visit)
{
	this->visit1 = visit;
}

bool Node::getVisit1()
{
	return visit1;
}

void Node::setVisit2(bool visit)
{
	this->visit2 = visit;
}

bool Node::getVisit2()
{
	return visit2;
}

void Node::SetParent(Node* p)
{
	parent = p;
}


void Node::SetValue( int value)
{
	this->value = value;
}

void Node::SetLine(int line)
{
	this->line = line;
}


void Node::SetColumn(int col)
{
	this->column = col;
}

int Node::GetValue()
{
	return value;
}


int Node::GetLine()
{
	return line;
}


int Node::GetColumn()
{
	return column;
}

Node* Node::GetParent()
{
	return parent;
}

void Node::AddEdge2Outgoing(Edge* pe) {
	outgoing.push_back(pe);
}

vector <Edge*> Node::GetOutgoing()
{
	return outgoing;
}

int Node::GetG() { // distance from start
	return g; 
}

void Node::SetG(int g) {
	this->g = g;
}


double Node::Distance(Node* end)  // distance to end
{
	double x1, x2, y1, y2;

	x1 = this->GetColumn();
	y1 = this->GetLine();
	x2 = end->GetColumn();
	y2 = end->GetLine();

	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void Node::SetH(Node* end)
{
	this->h = Distance(end);
}

double Node::GetH()
{
	return h;
}

double Node::GetF()
{
	return f;
}

void Node::SetF()
{
	f = h + g;
}


