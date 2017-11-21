#pragma once
#include "Object.h"
#include "Point.h"
#include <ostream>
#include <vector>

class Point
{
public:
	Point();
	Point(int x, int y, Object object);
	virtual ~Point();

	unsigned int distance(const Point &B) const;
	std::vector<Point> getCablesToB(const Point & ptB) const;
	bool voisinDe(const Point & ptB) const;
	Point closestCable(const std::vector<Point> & listCables) const;

	inline int getCoordX() const { return coordX; }
	inline int getCoordY() const { return coordY; }
	inline Object getType() const { return type; }
	inline void setType(const Object &obj) { type = obj; }

	friend bool operator==(Point const& ptA, Point const& ptB);
	friend std::ostream& operator<<(std::ostream& os, const Point& p);
protected:
	int coordX;
	int coordY;
	Object type;
};

