#pragma once
class Edge
{
public:
	Edge();
	Edge(int index1, int index2, double cost);
	~Edge();
	int GetFirst();
	int GetSecond();
	double GetCost();
private:
	int first, second;
	double cost;

};

