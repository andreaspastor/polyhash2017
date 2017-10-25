#pragma once

#include "Object.h"
#include "Point.h"
#include <vector>

class ProblemData
{
public:
	ProblemData();
	~ProblemData();
	void ParseFile(const char* filename);
	void Resolution();
	long calculMaxMoney();
	friend std::ostream& operator<<(std::ostream& os, const ProblemData& data);
	Point operator()(const unsigned int row, const unsigned int col);

	int getRow();
	int getCol();
	int getRouterRange();//besoin que le fonction retourne int pour la fonction scoreRouters
	unsigned int getConnectPrice();
	unsigned int getRouterPrice();
	unsigned int getMaxBudget();
	unsigned int getBackboneRow();
	unsigned int getBachboneCol();
protected:
	int row;
	int col;
	int routerRange;
	unsigned int connectPrice;
	unsigned int routerPrice;
	unsigned int maxBudget;
	unsigned int backboneRow;
	unsigned int backboneCol;
	std::vector<std::vector<Point>> map;
};

