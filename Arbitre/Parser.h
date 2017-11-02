#pragma once

#include "../Resolution/Point.h"
#include <vector>

//Décommenter pour tester
//#define DEBUG

class Parser {
public:
	Parser();
	~Parser();
	void ParseRule(const char*);
	void ParseAnswer(const char*);
	Point operator()(const unsigned int row, const unsigned int col);

	bool areRoutersConnectedToBackbone();
	bool areRoutersInWalls();
	bool isBudgetRespected();

	int calculeScore();

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

	int numberOfCellsConnected;
	std::vector<Point> cells;
	int numberOfRouters;
	std::vector<Point> routers;
	int numberOfTargetCells;
	int budgetCalculated;
	
	std::vector<std::vector<Point>> map;
};