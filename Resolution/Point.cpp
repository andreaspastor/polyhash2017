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

unsigned int Point::distance(const Point &ptB) 
{
	int x = std::abs(coordX - ptB.coordX);
	int y = std::abs(coordY - ptB.coordY);
	return std::fmin(x, y) + std::abs(x - y);
}

std::vector<Point> Point::getCablesToB(Point & ptB)
{
	std::vector<Point> listCables;
	int deltaX = std::abs(coordX - ptB.coordX);
	int deltaY = std::abs(coordY - ptB.coordY);
	int nbOblique = std::fmin(deltaX, deltaY);
	int nbVertical = std::abs(deltaX - deltaY);

	int signX = ((coordX - ptB.coordX) < 0) ? 1 : -1;
	int signY = ((coordY - ptB.coordY) < 0) ? 1 : -1;

	//ajout des cables obliques
	for (int i = 1; i <= nbOblique; i++) {
		listCables.push_back(Point(coordX + signX * i, coordY + signY * i, CABLE));
	}
	
	int axeX = ((deltaX - deltaY) > 0) ? 1 : 0;
	int axeY = (axeX == 1) ? 0 : 1;
	for (int j = nbVertical - 1; j >= 0; j--) {
		listCables.push_back(Point(ptB.coordX - j * signX * axeX, ptB.coordY - j * signY * axeY, CABLE));
	}
	return listCables;
}

bool Point::voisinDe(const Point & ptB)
{
	int deltaX = std::abs(ptB.coordX - coordX);
	int deltaY = std::abs(ptB.coordY - coordY);
	if (deltaX > 1 || deltaY > 1) {
		return false;
	}
	if (deltaX == 0 && deltaY == 0) {
		return false;
	}
	return true;
}

bool operator==(Point const & ptA, Point const & ptB)
{
	if (ptA.coordX == ptB.coordX && ptA.coordY == ptB.coordY && ptA.type == ptB.type) {
		return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
	os << "Le point a pour coordonnées (" << p.coordX << ", " << p.coordY << ")" << std::endl;
	os << "Le type du point est " << p.type << std::endl;
	return os;
}