#include "Graph.h"
#include <climits>
#include <iostream>


Graph::Graph()
{
}


Graph::~Graph()
{
}

Graph::Graph(const std::vector<Point>& routers)
{
	int x = 0;
	size = routers.size();
	for (auto &routerA : routers) {
		graph.push_back(std::vector<int>());
		for (auto &routerB : routers) {
			graph[x].push_back(routerA.distance(routerB));
		}
		x++;
	}
}

int Graph::minDist(const std::vector<int>& dist, const std::vector<bool>& isConnected)
{
	int minDist = INT_MAX;
	int minDist_index = 0;
	for (int x = 0; x < size; x++) {
		if (dist[x] < minDist && isConnected[x] == false) {
			minDist = dist[x];
			minDist_index = x;
		}
	}
	weight += minDist;
	return minDist_index;
}

void Graph::printSolution(const std::vector<int>& parent) const
{
	for (int x = 1; x < size; x++) {
		std::cout << parent[x] << " - " << x << " : " << graph[x][parent[x]] << std::endl;
	}
}

void Graph::resolve() {
	std::vector<int> dist(size, INT_MAX);
	std::vector<bool> isConnected(size, false);
	dist[0] = 0;
	parent.resize(size);
	parent[0] = -1;

	for (int h = 0; h < size; h++) {
		int x = minDist(dist, isConnected);
		isConnected[x] = true;
		for (int y = 0; y < size; y++) {
			if (graph[x][y] > 0 && isConnected[y] == false && dist[y] > graph[x][y]) {
				dist[y] = graph[x][y];
				parent[y] = x;
			}
		}
	}
#ifdef DEBUG
	//printSolution(parent);
	std::cout << weight << std::endl;
#endif;
}


