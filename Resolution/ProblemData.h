#pragma once

#include "Object.h"
#include "Point.h"
#include "Graph.h"
#include <vector>

using namespace std;

class ProblemData
{
public:
	ProblemData();
	~ProblemData();
	void ParseFile(const char* filename);
	void dumpInFile(const char* filename, vector<Point> & routers, const vector<Point> & listeCables);
	vector<Point> depotRouter();
	vector<Point> getRepartition(vector<Point>& routers, const vector<int> & parent);
	bool isCover(Point & ptA, Point & ptB);
	long scoreRouters(const vector<Point>& routers);
	long calculMaxMoney();
	friend ostream& operator<<(ostream& os, const ProblemData& data);
	Point operator()(const unsigned int row, const unsigned int col);

	int getRow();
	int getCol();
	int getRouterRange();//besoin que le fonction retourne int pour la fonction scoreRouters
	unsigned int getConnectPrice();
	unsigned int getRouterPrice();
	unsigned int getMaxBudget();
	unsigned int getBackboneRow();
	unsigned int getBackboneCol();
protected:
	int row;
	int col;
	int routerRange;
	unsigned int connectPrice;
	unsigned int routerPrice;
	unsigned int maxBudget;
	unsigned int backboneRow;
	unsigned int backboneCol;
	vector<vector<Point>> map;
};

void sorting(const vector<Point> & listeRef, vector<Point> & liste, const Point & ptCentre);
