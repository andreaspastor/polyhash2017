#pragma once

#include "Object.h"
#include "Point.h"
#include "Graph.h"
#include <vector>

//Décommenter  la ligne pour avoir les affichages
//#define DEBUG

//Classe principale de la résolution
class ProblemData
{
public:
	ProblemData();
	virtual ~ProblemData();

	void ParseFile(const char* filename);
	void dumpInFile(const char* filename);
	long potentielWifi(int x, int y) const;
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
	//Ensemble des informations sur le problème à resoudre
	int row;
	int col;
	int routerRange;
	unsigned int connectPrice;
	int routerPrice;
	int maxBudget;
	int maxBudgetInit;
	int backboneRow;
	int backboneCol;

	//Stockage des informations d'origine
	std::vector<std::vector<Point>> mapEntree;
	//Modification au fur et à mesure du depot des routeurs de la map d'origine
	std::vector<std::vector<Point>> mapSortie;
	//Map de recherche pour la couverture des routeurs (potentiel de point gagné pour chaque routeur)
	std::vector<std::vector<double>> mapSearchCov;
	//Map de recherche pour le coût de cablage de chaque cellule
	std::vector<std::vector<double>> mapSearchCab;

	//Stockage de la solution
	std::vector<Point> routers;
	std::vector<Point> cables;
	std::vector<Point> cablesSorted;//Cables rangés selon les règles de l'énoncée
};

void sorting(const std::vector<Point> & listeRef, std::vector<Point> & liste, const Point & ptCentre);


struct CompPoint {
	bool operator() (const Point& lhs, const Point& rhs) const
	{
		Point backbone(Point::backboneRow, Point::backboneCol, VIDE);
		return lhs.distance(backbone) <= rhs.distance(backbone);
	}
};