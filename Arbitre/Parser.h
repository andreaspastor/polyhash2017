#pragma once

#include "../Resolution/Point.h"
#include "../Resolution/Object.h"
#include <vector>
#include <unordered_map>
#include <utility>

//D�commenter pour tester
//#define DEBUG

struct HashPair {
	size_t operator()(const std::pair<unsigned int, unsigned int>& p) const {
		return (p.first + p.second) * (p.first + p.second + 1) / 2 + p.second;
	}
};

class Parser {
public:
	Parser();
	virtual ~Parser();
	void ParseRule(const char*);
	void ParseAnswer(const char*);
	void initialiseMapSolution();
	void setPointMapSolution(const Point &p);
	void coverCellsMap();

	Point& getPointMapSolution(unsigned int x, unsigned int y);

	bool isRouterCoveringCell(int ptAx, int ptAy, int ptBx, int ptBy) const;

	bool areRoutersConnectedToBackbone();
	bool areRoutersInWalls() const;
	bool isBudgetRespected();
	bool areAllRulesRespected();
	int computeScore() const;

	//besoin que le fonction retourne int pour la fonction scoreRouters
	inline Point operator()(unsigned int row, unsigned int col) const { return map[row][col]; }
	inline unsigned int getRouterRange() const { return routerRange; }
	inline unsigned int getRow() const { return row; }
	inline unsigned int getCol() const { return col; }
	inline unsigned int getConnectPrice() const { return connectPrice; }
	inline unsigned int getRouterPrice() const { return routerPrice; };
	inline unsigned int getMaxBudget() const { return maxBudget; }
	inline unsigned int getBackboneRow() const { return backboneRow; }
	inline unsigned int getBackboneCol() const { return backboneCol; }

protected:
	unsigned int row;
	unsigned int col;
	unsigned int routerRange;
	unsigned int connectPrice;
	unsigned int routerPrice;
	unsigned int maxBudget;
	unsigned int backboneRow;
	unsigned int backboneCol;

	int numberOfCellsConnected;
	int numberOfRouters;
	int numberOfTargetCells;
	int realNumberOfCellsConnected;
	int budgetCalculated;

	std::vector<Point> cells;
	std::vector<Point> routers;
	std::vector<std::vector<Point>> map;
	/*Reconstruction de la map petit a petit pour vérifier la solution*/
	std::unordered_map<std::pair<unsigned int, unsigned int>, Point, HashPair> map_solution;
};

