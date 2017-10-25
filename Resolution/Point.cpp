#include "Point.h"
#include <cmath>


Point::Point()
{
}


Point::~Point()
{
}

int Point::getCoordX()
{
	return coordX;
}

int Point::getCoordY()
{
	return coordY;
}

Object Point::getType()
{
	return type;
}

void Point::setType(Object obj)
{
	type = obj;
}

Point::Point(int x, int y, Object object)
{
	coordX = x;
	coordY = y;
	type = object;
}

unsigned int Point::distance(const Point &ptB) {
	int x = std::abs(coordX - ptB.coordX);
	int y = std::abs(coordY - ptB.coordY);
	return std::fmin(x, y) + std::abs(x - y);
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
	os << "Le point a pour coordonnées (" << p.coordX << ", " << p.coordY << ")" << std::endl;
	os << "Le type du point est " << p.type << std::endl;
	return os;
}