#include "Graph.h"
#include <climits>
#include <iostream>

using namespace std;

Graph::Graph()
{
}


Graph::~Graph()
{
}

Graph::Graph(const Point & backbone, const vector<Point>& routers)
{
	int x = 0;
	size = routers.size();
	for (auto routerA : routers) {
		graph.push_back(vector<int>());
		for (auto routerB : routers) {
			graph[x].push_back(routerA.distance(routerB));
		}
		x++;
	}
}

int Graph::minDist(const vector<int>& dist, const vector<bool>& isConnected)
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

void Graph::printSolution(const vector<int>& parent)
{
	for (int x = 1; x < size; x++) {
		cout << parent[x] << " - " << x << " : " << graph[x][parent[x]] << endl;
	}
}

void Graph::resolve() {
	vector<int> dist(size, INT_MAX);
	vector<bool> isConnected(size, false);
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
	//printSolution(parent);
	cout << weight << endl;
}

long Graph::getWeight()
{
	return weight;
}

int Graph::getSize()
{
	return size;
}
