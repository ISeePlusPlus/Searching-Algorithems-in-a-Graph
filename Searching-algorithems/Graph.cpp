#include "Graph.h"

Graph::Graph()
{
}


Graph::~Graph()
{
}



void Graph::AddNode(Node* pn)
{
	nodes.push_back(pn);
}

void Graph::AddEdge(Edge* pe)
{
	edges.push_back(pe);
}

Node* Graph::GetNode(int index)
{
	return nodes[index];
}

Edge* Graph::GetEdge(int index)
{
	return edges[index];
}

vector <Node*> Graph::GetNodes()
{
	return nodes;
}
