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
	vector<Point> getCablesAToB(const int & fils, const int & parent);
	vector<Point> getRepartition(const ProblemData & data);
	long getWeight();
	int getSize();
protected:
	vector<vector<int>> graph;
	vector<int> parent;
	long weight;
	int size;
};

