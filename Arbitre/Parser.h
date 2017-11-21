#pragma once

#include "../Resolution/Point.h"
#include <vector>

//Décommenter pour tester
//#define DEBUG

class Parser {
public:
	Parser();
	virtual ~Parser();
	void ParseRule(const char*);
	void ParseAnswer(const char*);
	

	bool areRoutersConnectedToBackbone() const;
	bool areRoutersInWalls() const;
	bool isBudgetRespected() const;
	bool areAllRulesRespected() const;
	int computeScore() const;

	//besoin que le fonction retourne int pour la fonction scoreRouters
	inline Point operator()(unsigned int row, unsigned int col) const { return map[row][col]; }
	inline int getRouterRange() const { return routerRange; }
	inline int getRow() const { return row; }
	inline int getCol() const { return col; }
	inline unsigned int getConnectPrice() const { return connectPrice;  }
	inline unsigned int getRouterPrice() const { return routerPrice;  };
	inline unsigned int getMaxBudget() const { return maxBudget;  }
	inline unsigned int getBackboneRow() const { return backboneRow;  }
	inline unsigned int getBackboneCol() const { return backboneCol;  }

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
	int numberOfRouters;
	int numberOfTargetCells;
	
	std::vector<Point> cells;
	std::vector<Point> routers;
	std::vector<std::vector<Point>> map;
};