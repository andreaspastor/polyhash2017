#pragma once

#include "Object.h"
#include <vector>

class ProblemData
{
public:
	ProblemData();
	~ProblemData();
	void ParseFile(const char* filename);
	void Resolution();
	friend std::ostream& operator<<(std::ostream& os, const ProblemData& data);
	Object operator()(const unsigned int row, const unsigned int col);
protected:
	unsigned int row;
	unsigned int col;
	unsigned int routerRange;
	unsigned int connectPrice;
	unsigned int routerPrice;
	unsigned int maxBudget;
	unsigned int backboneRow;
	unsigned int backboneCol;
	std::vector<std::vector<Object>> map;
};

