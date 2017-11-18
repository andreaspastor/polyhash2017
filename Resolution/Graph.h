#pragma once
#include <vector>
#include "Object.h"
#include "Point.h"
#include "ProblemData.h"

class Graph
{
public:
	Graph();
	~Graph();
	Graph(const std::vector<Point>& routers);
	void resolve();
	int minDist(const std::vector<int>& dist, const std::vector<bool>& isConnected);
	void printSolution(const std::vector<int>& parent);
	long getWeight();
	int getSize();
	std::vector<int> parent;
protected:
	std::vector<std::vector<int>> graph;
	long weight;
	int size;
};

