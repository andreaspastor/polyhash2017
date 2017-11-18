#pragma once

#include "Object.h"
#include "Point.h"
#include "Graph.h"
#include <vector>

//Décommenter  la ligne pour avoir les affichages
//#define DEBUG

class ProblemData
{
public:
	ProblemData();
	~ProblemData();
	void ParseFile(const char* filename);
	void dumpInFile(const char* filename);
	int potentielWifi(const int & x, const int & y);
	int distance(const int & x, const int & y);
	void depotRouter();
	std::vector<Point> getRepartition(const std::vector<int> & parent);
	bool isCover(const Point & ptA, const Point & ptB);
	bool isCover(const int & ptAx, const int & ptAy, const int & ptBx, const int & ptBy);
	long scoreRouters();
	long calculMaxMoney();
	friend std::ostream& operator<<(std::ostream& os, const ProblemData& data);
	Point operator()(const unsigned int row, const unsigned int col);

	int getRow() const;
	int getCol() const;
	int getRouterRange() const;//besoin que le fonction retourne int pour la fonction scoreRouters
	unsigned int getConnectPrice() const;
	int getRouterPrice() const;
	unsigned int getMaxBudget() const;
	int getBackboneRow() const;
	int getBackboneCol() const;
	int getNbRouters() const;
	std::vector<Point> getRouters() const;
	int getNbCables() const;
	std::vector<Point> getCables() const;
protected:
	int row;
	int col;
	int routerRange;
	unsigned int connectPrice;
	int routerPrice;
	unsigned int maxBudget;
	int backboneRow;
	int backboneCol;
	std::vector<std::vector<Point>> map;
	std::vector<Point> routers;
	std::vector<Point> cables;
};

void sorting(const std::vector<Point> & listeRef, std::vector<Point> & liste, const Point & ptCentre);
