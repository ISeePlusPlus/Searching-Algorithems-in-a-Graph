#include "Edge.h"



Edge::Edge()
{
}

Edge::Edge(int index1,int index2,double cost)
{
	first = index1;
	second = index2;
	this->cost = cost;
}


Edge::~Edge()
{
}

int Edge::GetFirst() {
	return first;
}

int Edge::GetSecond() {
	return second;
}

double Edge::GetCost() {
	return cost;
}
