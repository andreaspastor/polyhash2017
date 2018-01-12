#pragma once
#include "Object.h"
#include "Point.h"
#include <ostream>
#include <vector>


//Classe stockant les informations d'une cellule de la map
class Point {
public:

	/** Constructor
	*/
	Point();
	
	/** Constructor with given parameters
	
		@param x int Coordinate x
		@param y int Coordinate y
		@param object Object The object type
	*/
	Point(int x, int y, Object object);
	
	/** The destructor
	*/
	virtual ~Point();

	/** Compute distance between 2 points
	
		@param B Point The point to compare with
		@return int The distance
	*/
	unsigned int distance(const Point &B) const;
	
	/** Generate the way for cables between 2 points
	
		@param ptB Point the ending point for the way
		@return vector<Point> The list of points
	*/
	std::vector<Point> getCablesToB(const Point & ptB) const;
	
	/** Generate the way for cables between 2 points with oblique method
	
		@param ptB Point the ending point for the way
		@return vector<Point> The list of points
	*/
	std::vector<Point> getCablesDiagTo(const Point & ptB) const;
	
	/** Check if 2 points are neighbours
	
		@param ptB Point The point to compare with
		@return boolean True if neighbour
	*/
	bool voisinDe(const Point & ptB) const;
	
	/** Look at the closest point
		
		@param listCables vector<Point> The list of cables
		@return Point The closest point
	*/
	Point closestCable(const std::vector<Point> & listCables) const;

	/** Get the coordinate x
	
		@return int The coordinate x
	*/
	inline int getCoordX() const { return coordX; }

	/** Get the coordinate y
	
		@return int The coordinate y
	*/
	inline int getCoordY() const { return coordY; }

	/** Get the type
	
		@return int The type
	*/
	inline Object getType() const { return type; }

	/** Set the type
	
		@param obj Object The type
	*/
	inline void setType(const Object &obj) { type = obj; }

	/** Redefinition of operator ==
	*/
	friend bool operator==(Point const& ptA, Point const& ptB);
	
	/** Redefinition of operator <<
	*/
	friend std::ostream& operator<<(std::ostream& os, const Point& p);

	static int backboneRow;
	static int backboneCol;
protected:
	//Stockage de l'emplacement de la cellule et de son type
	int coordX;
	int coordY;
	Object type;
};

