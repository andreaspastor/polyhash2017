#pragma once
#include "Object.h"
#include <ostream>

class Point
{
public:
	Point();
	Point(int x, int y, Object object);
	unsigned int distance(Point &B);
	~Point();
	int coordX;
	int coordY;
	Object type;
	friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

