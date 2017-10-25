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

	unsigned int getRow();
	unsigned int getCol();
	unsigned int getRouterRange();
	unsigned int getConnectPrice();
	unsigned int getRouterPrice();
	unsigned int getMaxBudget();
	unsigned int getBackboneRow();
	unsigned int getBachboneCol();
protected:
	unsigned int row;
	unsigned int col;
	unsigned int routerRange;
	unsigned int connectPrice;
	unsigned int routerPrice;
	unsigned int maxBudget;
	unsigned int backboneRow;
	unsigned int backboneCol;
	std::vector<std::vector<Point>> map;
};

