#include "Point.h"
#include <cmath>


Point::Point()
{
}


Point::~Point()
{
}


Point::Point(int x, int y)
{
	coordX = x;
	coordY = y;
}

unsigned int Point::distance(Point &ptB) {
	int x = std::abs(coordX - ptB.coordX);
	int y = std::abs(coordY - ptB.coordY);
	return std::fmin(x, y) + std::abs(x - y);
}