#pragma once

#include "Object.h"
#include "Point.h"
#include <vector>

//Décommenter  la ligne pour avoir les affichages
//#define DEBUG

//Classe principale de la résolution
class ProblemData {
public:

	/** Constructor
	*/
	ProblemData();
	
	/** Destructor
	*/
	virtual ~ProblemData();

	/** Parse the file given a file name
	
		@param filename char The file name
	*/
	void ParseFile(const char* filename);
	
	/** Dump solution data in file
	
		@param filename The file name
	*/
	void dumpInFile(const char* filename);
	
	/** Compute the wifi score given coordinates
	
		@param x int Coordonate x
		@param y int Coordonate y
		@return long The score
	*/
	long potentielWifi(int x, int y) const;
	
	/** Compute the shortest distance between coordinates and nearest cable
	
		@param x int Coordinate x
		@param y int Coordinate y
		@return int The distance
	*/
	int distance(int x, int y) const;
	
	/** Compute the shortest distance between point and list of cables
	
		@param x int Coordinate x
		@param y int Coordinate y
		@param newCables vector<Point> List of cables
		@return int The shortest distance 
	*/
	int distanceNewCables(int x, int y, const std::vector<Point>& newCables) const;
	
	/** Put a router based on our strategy
	*/
	void depotRouter();
	
	/** Check if a cell is covered if a router is placed (Point version)
	
		@param ptA Point The cell
		@param ptB Point The Router
		@return boolean True if it's covered
	*/
	bool isCover(const Point & ptA, const Point & ptB) const;
	
	/** Check if a cell is covered if a router is placed (int version)
	
		@param ptAx int Cell's coordinate x
		@param ptAy int Cell's coordinate y
		@param ptBx Point Router's coordinate x
		@param ptBy Point Rotuer's coordinate y
		@return boolean True if it's covered
	*/
	bool isCover(int ptAx, int ptAy, int ptBx, int ptBy) const;
	
	/** Compute score based on list of routers
	
		@return long The score
	*/
	long scoreRouters();
	
	/** Compute the maximum possible if the whole map is covered
	
		@return long Maximum money
	*/
	long calculMaxMoney() const;

	/** Redefine operator <<
	*/
	friend std::ostream& operator<<(std::ostream& os, const ProblemData& data);

	/** Redefine operator ()
	*/
	inline Point operator()(unsigned int row, unsigned int col) const { return mapEntree[row][col]; }
	
	/** Get the router range
	
		@return int The router range
	*/
	inline int getRouterRange() const { return routerRange; }
	
	/** Get the row
	
		@return int The row
	*/
	inline int getRow() const { return row; }
	
	/** Get the colum,
	
		@return int The column
	*/
	inline int getCol() const { return col; }
	
	/** Get the price of connection
	
		@return int The connection's price
	*/
	inline unsigned int getConnectPrice() const { return connectPrice; }
	
	/** Get the router price
	
		@return int The router price
	*/
	inline int getRouterPrice() const { return routerPrice; };
	
	/** Get the budget maximum
	
		@return int The maximum budget
	*/
	inline int getMaxBudget() const { return maxBudget; }
	
	/** Get the backbone row
	
		@return int The backbone row
	*/
	inline int getBackboneRow() const { return backboneRow; }
	
	/** Get the backbone column
	
		@return int The backbone column
	*/
	inline int getBackboneCol() const { return backboneCol; }
	
	/** Get the number of routers
	
		@return int The number of routers
	*/
	inline int getNbRouters() const { return routers.size(); }
	
	/** Get the number of cables
	
		@return int The number of cables
	*/
	inline int getNbCables() const { return cables.size(); }
	
	/** Get the list of cables
	
		@return int The list of cables
	*/
	inline std::vector<Point> getCables() const { return cables; }
	
	/** Get the list of routers
	
		@return int The list of routers
	*/
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