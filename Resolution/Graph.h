#pragma once
#include <vector>
#include "Object.h"
#include "Point.h"
#include "ProblemData.h"

class Graph
{
public:
	Graph();
	virtual ~Graph();
	Graph(const std::vector<Point>& routers);
	void resolve();
	int minDist(const std::vector<int>& dist, const std::vector<bool>& isConnected);
	void printSolution(const std::vector<int>& parent) const;

	std::vector<int> parent;

	inline long getWeight() const { return weight; }
	inline int getSize() const { return size; }

protected:
	std::vector<std::vector<int>> graph;
	long weight;
	int size;
};

