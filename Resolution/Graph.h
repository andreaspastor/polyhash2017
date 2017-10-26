#pragma once
#include <vector>
#include "Object.h"
#include "Point.h"
#include "ProblemData.h"

using namespace std;

class Graph
{
public:
	Graph();
	~Graph();
	Graph(const vector<Point>& routers);
	void resolve();
	int minDist(const vector<int>& dist, const vector<bool>& isConnected);
	void printSolution(const vector<int>& parent);
	long getWeight();
	int getSize();
	vector<int> parent;
protected:
	vector<vector<int>> graph;
	long weight;
	int size;
};

