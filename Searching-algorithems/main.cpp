#include<GL/glut.h>
#include <math.h>
#include <time.h>
#include "Node.h"
#include <vector>
#include "graph.h"
#include <queue>
#include "CompareNodes.h"
#include <iostream>

using namespace std;

const int W = 600; // window width
const int H = 600; // window height
const int MSZ = 100;
const int NUM_NODES = 1000;
const int NUM_EDGES = 3000;

bool run_dfs=false, run_bfs = false, run_bidirectional_bfs = false, AStar_run =false;

Graph graph;

vector <Node*> gray; // gray nodes
vector <Node*> gray2; 

priority_queue <Node*, vector<Node*>, CompareNodes> pq;


void CreateGraph();
void displayGraph();


void init()
{

	srand(time(0)); // pseudo randomizer

	glClearColor(0.7, 0.7, 0.7, 0);

	CreateGraph();
	glutDisplayFunc(displayGraph); // refresh function

	glOrtho(-1, 1, -1, 1, -1, 1);

}


double Distance(int index1, int index2)  // minimize edge lenght
{
	double x1, x2, y1, y2;

	x1 = graph.GetNode(index1)->GetColumn();
	y1 = graph.GetNode(index1)->GetLine();
	x2 = graph.GetNode(index2)->GetColumn();
	y2 = graph.GetNode(index2)->GetLine();

	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void CreateGraph()
{
	int i,j,k;
	Node* pn;
	Edge* pe1,*pe2;
	double tmp;

	// add random Nodes
	for (k = 0; k < NUM_NODES; k++)
	{
		pn = new Node();
		pn->SetLine(rand() % MSZ);
		pn->SetColumn(rand() % MSZ);
		pn->SetValue(SPACE);

		graph.AddNode(pn);

	}
	// add random Edges
	for (k = 0; k < NUM_EDGES; k++)
	{
		do {
			i = rand() % NUM_NODES;
			j = rand() % NUM_NODES;
			tmp = Distance(i, j);
		} while (tmp > 6);
		// add edges to graph
		pe1 = new Edge(i, j, tmp);
		graph.AddEdge(pe1);
		pe2 = new Edge(j, i, tmp);
		graph.AddEdge(pe2);
		// add edges to outgoing of nodes
		graph.GetNode(i)->AddEdge2Outgoing(pe1);
		graph.GetNode(j)->AddEdge2Outgoing(pe2);
	}

	graph.GetNode(0)->SetValue(START);
	//graph.GetNode(0)->SetParent(graph.GetNode(0));
	graph.GetNode(graph.GetNodes().size()-1)->SetValue(TARGET);
	// add start node to priority queue
	gray.push_back(graph.GetNode(0));
	gray2.push_back(graph.GetNode(graph.GetNodes().size() - 1));
	pq.push(graph.GetNode(0));


}

void DrawNode(int i, int j)
{
	double sz, x, y;

	sz = 2.0 / MSZ;
	x = j * sz - 1;
	y = i * sz - 1;

	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d(x + sz, y);
	glVertex2d(x + sz, y + sz);
	glVertex2d(x, y + sz);

	glEnd();

}

void DrawEdge(Edge* pe)
{
	double x1, y1, x2, y2,sz;

	sz = 2.0 / MSZ;

	x1 = graph.GetNode(pe->GetFirst())->GetColumn() * sz -1+sz/2;
	y1 = graph.GetNode(pe->GetFirst())->GetLine()* sz - 1 + sz / 2;
	x2 = graph.GetNode(pe->GetSecond())->GetColumn()* sz - 1 + sz / 2;
	y2 = graph.GetNode(pe->GetSecond())->GetLine()* sz - 1 + sz / 2;

	glColor3d(0, 0, 0); // black

	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();


}


void DrawGraph()
{
	int i;
	// draw nodes
	for (i = 0; i < NUM_NODES; i++)
	{
		switch (graph.GetNode(i)->GetValue())
		{
		case SPACE:
			glColor3d(0, 0, 0.5); // blue
			break;
		case WALL:
			glColor3d(0.4, 0, 0); // dark red
			break;
		case START:
			glColor3d(0.4, 0.8, 1); // light blue
			break;
		case TARGET:
			glColor3d(1, 0, 0); // red
			break;
		case GRAY:
			glColor3d(0, 1, 0); // light green
			break;
		case BLACK:
			glColor3d(0, 0.7, 0.3); // dark green
			break;
		case PATH:
			glColor3d(1, 0, 1); // magenta
			break;
		}//switch
		DrawNode(graph.GetNode(i)->GetLine(), graph.GetNode(i)->GetColumn());
		for (auto it : graph.GetNode(i)->GetOutgoing())
			DrawEdge(it);
	}// for
}


void RestorePathInGraph(Node* first, int origin)
{
	Node* current =first;

	while (current->GetValue() != origin)
	{
		// paint it to PATH color
		current->SetValue(PATH);
		current = current->GetParent();
	}

}

void BfsIteration(vector <Node*>&gray,int origin, int destination, bool direction)
{
	Node* current, * next;

	if (gray.empty()) // there is no way to target
	{
		cout << "There is no path" << endl;
		run_bidirectional_bfs = false;
		return;
	}

	current = gray.front();
	gray.erase(gray.begin());

	if (current->GetValue() != origin)
		current->SetValue(BLACK);

	// check all non-black neighbors of current
	for (auto it : current->GetOutgoing())
	{
		next = graph.GetNode(it->GetSecond());
		int value = next->GetValue();

		if ((next->getVisit1() == true && direction == false) || next->GetValue() == destination  //if path is len one, next == destination
			||(next->getVisit2() == true && direction == true)) 
		{
			cout << "Target was found" << endl;
			run_bidirectional_bfs = false;
			run_bfs = false;
			RestorePathInGraph(current, origin);
			RestorePathInGraph(next, destination);
			return;
		}

		if (direction)
		{
			next->setVisit1(true);
		}
		else
		{
			next->setVisit2(true);
		}

		if (value != BLACK)
		{
			// next is the Node that is the secondend of edge it
			if (value == SPACE || value == destination)
			{
				gray.push_back(next);
				next->SetParent(current);
				value == SPACE ? next->SetValue(GRAY) : 0 ;  //do nothing if target
			}
		}

	}
}

void DfsIteration(vector <Node*>& gray, int origin, int destination)
{
	Node* current, *next;

	if (gray.empty()) // there is no way to target
	{
		cout << "There is no path" << endl;
		run_dfs = false;
		return;
	}

	// kind of pop
	current = gray.back();
	gray.pop_back();

	if (current->GetValue() != origin)
		current->SetValue(BLACK);

	for (auto it : current->GetOutgoing())
	{
		next = graph.GetNode(it->GetSecond());
		int value = next->GetValue();

		if ((next->getVisit1() == true ) || next->GetValue() == destination ) //if path is len one, next == destination
		{
			cout << "Target was found" << endl;
			run_dfs = false;
			RestorePathInGraph(current, origin);
			return;
		}

		if (value != BLACK)
		{
			// next is the Node that is the secondend of edge it
			if (value == SPACE || value == destination)
			{
				gray.push_back(next);
				next->SetParent(current);
				value == SPACE ? next->SetValue(GRAY) : 0;  //do nothing if target
			}
		}

	}


	
}

void Bi_Bfs(vector <Node*>&gray, vector <Node*>&gray2, int start, int end)
{
	//direction of BFS, true from Start to target, false other way 
	bool direction = true;    
	BfsIteration(gray, start, end, direction);
	direction = false;
	BfsIteration(gray2, end, start, direction);
}




void UniformCostIteration()
{
	Node* current, * next;

	if (pq.empty())
	{
		cout << "There is no path" << endl;
		AStar_run = false;
		return;
	}

	current = pq.top();
	pq.pop();

	if (current->GetValue() == TARGET)
	{
		cout << "Target was found" << endl;
		AStar_run = false;
		RestorePathInGraph(current->GetParent(), START);
		return;
	}

	if (current->GetValue() != START)
		current->SetValue(BLACK);

	int newG = current->GetG() + 1;
	// check all non-black neighbors of current
	for (auto it : current->GetOutgoing())
	{
		// next is the Node that is the secondend of edge it
		next = graph.GetNode(it->GetSecond());
		int value = next->GetValue();
		if (value != BLACK)
		{
			// update G of next
			if (value == SPACE || // white target
				(value == TARGET))
			{
				// Add next  to PQ only if next was WHITE
				next->SetH(graph.GetNode(graph.GetNodes().size() - 1));
				next->SetG(newG);
				next->SetF();
				value == SPACE ? next->SetValue(GRAY) : 0;  //do nothing if target
				next->SetParent(current);
				pq.push(next);
			}

			else // compare it to its actual G
			{
				if (newG < next->GetG())
				{
					next->SetG(newG);
					next->SetF();
					next->SetParent(current);


				}
			}
		}

	}
}



void displayGraph()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawGraph();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void clearBoard(){
		int i;
		run_dfs = false;
		run_bfs = false;
		run_bidirectional_bfs = false;
		AStar_run = false;
		for (i = 1; i < NUM_NODES - 1; i++)
		{
			graph.GetNode(i)->Clear();
		}
		gray.clear();
		gray2.clear();
		while (!pq.empty()) {
			pq.pop();
		}
		gray.push_back(graph.GetNode(0));
		gray2.push_back(graph.GetNode(graph.GetNodes().size() - 1));
		pq.push(graph.GetNode(0));
}

void idle()
{
	if(run_bfs)
		BfsIteration(gray, START, TARGET, true);
	if (run_dfs)
		DfsIteration(gray, START, TARGET);
	if (AStar_run)
		UniformCostIteration();
	if (run_bidirectional_bfs)
		Bi_Bfs(gray, gray2, START, TARGET);
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{
	switch (choice) {
	case 1:
		run_bfs = true;
		break;
	case 2:
		run_dfs = true;
		break;
	case 3: //bi - bfs
		run_bidirectional_bfs = true;
		break;
	case 4:
		AStar_run = true;
		//		CreateGraph();
		//		glutDisplayFunc(displayGraph); // refresh function
		break;
	case 5:
		clearBoard();
		break;
	}
	
}
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("AI Example ");

	glutDisplayFunc(displayGraph); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens

	// menu
	glutCreateMenu(Menu);
	glutAddMenuEntry("BFS", 1);
	glutAddMenuEntry("DFS", 2);
	glutAddMenuEntry("Bi - BFS", 3);
	glutAddMenuEntry("A* ", 4);
	glutAddMenuEntry("Reset Board ", 5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();
}