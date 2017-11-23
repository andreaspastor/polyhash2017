#pragma once

#include "Object.h"
#include "Point.h"
#include "Graph.h"
#include <vector>

//D�commenter  la ligne pour avoir les affichages
//#define DEBUG

class ProblemData
{
public:
	ProblemData();
	virtual ~ProblemData();

	void ParseFile(const char* filename);
	void dumpInFile(const char* filename);
	int potentielWifi(int x, int y) const;
	int distance(int x, int y) const;
	int distanceNewCables(int x, int y, const std::vector<Point>& newCables) const;
	void depotRouter();
	std::vector<Point> getRepartition(const std::vector<int> & parent);
	bool isCover(const Point & ptA, const Point & ptB) const;
	bool isCover(int ptAx, int ptAy, int ptBx, int ptBy) const;
	long scoreRouters();
	long calculMaxMoney() const;
	friend std::ostream& operator<<(std::ostream& os, const ProblemData& data);

	inline Point operator()(unsigned int row, unsigned int col) const { return mapEntree[row][col]; }
	inline int getRouterRange() const { return routerRange; }
	inline int getRow() const { return row; }
	inline int getCol() const { return col; }
	inline unsigned int getConnectPrice() const { return connectPrice; }
	inline int getRouterPrice() const { return routerPrice; };
	inline int getMaxBudget() const { return maxBudget; }
	inline int getBackboneRow() const { return backboneRow; }
	inline int getBackboneCol() const { return backboneCol; }
	inline int getNbRouters() const { return routers.size(); }
	inline int getNbCables() const { return cables.size(); }
	inline std::vector<Point> getCables() const { return cables; }
	inline std::vector<Point> getRouters() const { return routers; }

protected:
	int row;
	int col;
	int routerRange;
	unsigned int connectPrice;
	int routerPrice;
	int maxBudget;
	int backboneRow;
	int backboneCol;
	std::vector<std::vector<Point>> mapEntree;
	std::vector<std::vector<Point>> mapSortie;
	std::vector<std::vector<double>> mapSearchCov;
	std::vector<std::vector<double>> mapSearchCab;
	std::vector<Point> routers;
	std::vector<Point> cables;
};

void sorting(const std::vector<Point> & listeRef, std::vector<Point> & liste, const Point & ptCentre);
