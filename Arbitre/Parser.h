#pragma once

#include "../Resolution/Point.h"
#include "../Resolution/Object.h"
#include <vector>
#include <unordered_map>
#include <utility>

//Uncomment for test
//#define DEBUG

struct HashPair {
	size_t operator()(const std::pair<unsigned int, unsigned int>& p) const {
		return (p.first + p.second) * (p.first + p.second + 1) / 2 + p.second;
	}
};

/** Main class for the arbitration
*/
class Parser {
public:

	/** Constructor
	*/
	Parser();
	
	/** Destructor
	*/
	virtual ~Parser();
	
	/** Parse the rule

		@param filename char The file name
	*/
	void ParseRule(const char*);
	
	/** Parse the answer file

		@param filename char The file name
	*/
	void ParseAnswer(const char*);
	
	/** Initialize the map for the solution
	*/
	void initialiseMapSolution();
	
	/** Set the points for the map

		@param p Point the Point to set
	*/
	void setPointMapSolution(const Point &p);
	
	/**	Count how many cells are being covered by every routers
	*/
	void coverCellsMap();

	/** Get the points for the map

		@param x int Coordonate x
		@param y int Coordonate y
		@return Point the Point of the given coordonates
	*/
	Point& getPointMapSolution(unsigned int x, unsigned int y);

	/** Test if a router cover a given cell

		@param ptAx int  Point A.x
		@param ptAy int  Point A.y
		@param ptBx int  Point B.x
		@param ptBy int  Point B.y
		@return boolean True if the router cover the cell
	*/
	bool isRouterCoveringCell(int ptAx, int ptAy, int ptBx, int ptBy) const;

	/** Function that checks if the routers are connected to the backbone

		@return boolean True the routeurs are connected
	*/
	bool areRoutersConnectedToBackbone();
	
	/** Check if routers are in walls or not

		@return boolean True if routers are in walls
	*/
	bool areRoutersNotInWalls() const;
	
	/** Check if budget is respected 

		@return boolean True si le budget est respecté (budged <= maxBudget)
	*/
	bool isBudgetRespected();
	
	/** Function to try every rules

		@return boolean True if all the rules are respected
	*/
	bool areAllRulesRespected();
	
	/**	Compute the final score

	@return int the final score
	*/
	int computeScore() const;

	/** Redifine the operator ()
	
		@return Point The row and column
	*/
	inline Point operator()(unsigned int row, unsigned int col) const { return map[row][col]; }
	
	/** Return the range of the router
	
		@return int The router's range
	*/
	inline unsigned int getRouterRange() const { return routerRange; }
	
	/** Get the row
	
		@return int The row
	*/
	inline unsigned int getRow() const { return row; }
	
	/** Get the column
	
		@return int The column
	*/
	inline unsigned int getCol() const { return col; }
	
	/** Get the connect price
	
		@return int The connect price
	*/
	inline unsigned int getConnectPrice() const { return connectPrice; }
	
	/** Get the router price
	
		@return int The router price
	*/
	inline unsigned int getRouterPrice() const { return routerPrice; };
	
	/** Get the max budget
	
		@return int The max budget
	*/
	inline unsigned int getMaxBudget() const { return maxBudget; }
	
	/** Get the backbone row
	
		@return int The backbone row
	*/
	inline unsigned int getBackboneRow() const { return backboneRow; }
	
	/** Get the backbone column
	
		@return int The backbone column
	*/
	inline unsigned int getBackboneCol() const { return backboneCol; }

protected:

	/** The row
	*/
	unsigned int row;
	
	/** The column
	*/
	unsigned int col;
	
	/** The router range
	*/
	unsigned int routerRange;
	
	/** The connect price
	*/
	unsigned int connectPrice;
	
	/** The router price
	*/
	unsigned int routerPrice;
	
	/** The max budget
	*/
	unsigned int maxBudget;
	
	/** The backbone row
	*/
	unsigned int backboneRow;
	
	/** The backbone column
	*/
	unsigned int backboneCol;

	
	/** The number of cells connected
	*/
	int numberOfCellsConnected;
	
	/** The number of routers
	*/
	int numberOfRouters;
	
	/** The number of target cells
	*/
	int numberOfTargetCells;
	
	/** The real number of connected cells
	*/
	int realNumberOfCellsConnected;
	
	/** The calculated budget
	*/
	int budgetCalculated;

	
	/** The array of cells
	*/
	std::vector<Point> cells;
	
	/** The array of routers
	*/
	std::vector<Point> routers;
	
	/** The array of point in the map
	*/
	std::vector<std::vector<Point>> map;
	
	/** The map solution
	*/
	std::unordered_map<std::pair<unsigned int, unsigned int>, Point, HashPair> map_solution;
};

