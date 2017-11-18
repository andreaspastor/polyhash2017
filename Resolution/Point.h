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
	unsigned int distance(const Point &B) const;
	std::vector<Point> getCablesToB(Point & ptB);
	bool voisinDe(const Point & ptB);
	Point closestCable(const std::vector<Point> & listCables);
	~Point();
	int getCoordX() const;
	int getCoordY() const;
	Object getType() const;
	void setType(Object obj);
	friend bool operator==(Point const& ptA, Point const& ptB);
	friend std::ostream& operator<<(std::ostream& os, const Point& p);
protected:
	int coordX;
	int coordY;
	Object type;
};

