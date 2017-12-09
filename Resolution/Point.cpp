#include "Point.h"
#include <cmath>
#include <iostream>

int Point::backboneCol = 0;
int Point::backboneRow = 0;

Point::Point()
{
}


Point::~Point()
{
}


Point::Point(int x, int y, Object object) : coordX(x), coordY(y), type(object) 
{
}

unsigned int Point::distance(const Point &ptB) const
{
	int x = std::abs(coordX - ptB.coordX);
	int y = std::abs(coordY - ptB.coordY);
	return std::fmin(x, y) + std::abs(x - y);
}

std::vector<Point> Point::getCablesToB(const Point &ptB) const
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

std::vector<Point> Point::getCablesDiagTo(const Point & ptB) const
{
	std::vector<Point> listCables;
	int x; //variable de parcours
	int direction;
	int x1 = coordX;
	int y1 = coordY;
	int x2 = ptB.coordX;
	int y2 = ptB.coordY;

	if (x1 != x2) { // ABSCISSES DIFFERENTES
		int a1 = -1;
		int b1 = y1 - a1 * x1;
		int a2 = 1;
		int b2 = y2 - a2 * x2;
		direction = x1 < x2 ? 1 : -1;

		//diagonales directes
		if (a1*x2 + b1 == y2) {
			x = x1 + direction;
			while (x != x2) {
				listCables.push_back(Point(x, a1*x + b1, CABLE));
				x = x + direction;
			}
		}
		else if (a2*x1 + b2 == y1) {
			x = x1 + direction;
			while (x != x2) {
				listCables.push_back(Point(x, a2*x + b2, CABLE));
				x = x + direction;
			}
		}
		else {
			int b1test = y1 + a1*x1;
			direction = y2 > x2 + b1test ? -1 : 1;
			if (std::abs(x1 - x2) % 2 != std::abs(y1 - y2) % 2) {
				x1 = x1 + direction;
				listCables.push_back(Point(x1, y1, CABLE));
				b1 = y1 - a1*x1;
			}
			bool findIntersec = false;
			x = x1;
			while (a1*x + b1 != a2*x + b2) {
				listCables.push_back(Point(x, a1*x + b1, CABLE));
				x = x + direction;
			}
			direction = x < x2 ? 1 : -1;
			while (x != x2) {
				listCables.push_back(Point(x, a2*x + b2, CABLE));
				x = x + direction;
			}
		}
	}
	else { // MEME ABSCISSES
		int diff = std::abs(y1 - y2);
		int nbDiag = diff / 2;
		direction = y1 < y2 ? 1 : -1;
		for (int i = 0; i < nbDiag; i++) {
			x1 = x1 + direction;
			y1 = y1 + direction;
			listCables.push_back(Point(x1, y1, CABLE));
		}
		
		if (diff % 2 == 1) {
			y1 = y1 + direction;
			listCables.push_back(Point(x1, y1, CABLE));
		}

		for (int i = 0; i < nbDiag; i++) {
			x1 = x1 - direction;
			y1 = y1 + direction;
			listCables.push_back(Point(x1, y1, CABLE));
		}
	}

	return listCables;
}

bool Point::voisinDe(const Point & ptB) const
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

Point Point::closestCable(const std::vector<Point>& listCables) const
{
	Point closest;
	int xx, yy, dist;
	int minDist = 9999;
	for (auto &cable : listCables) {
		xx = abs(coordX - cable.coordX);
		yy = abs(coordY - cable.coordY);
		dist = std::fmin(xx, yy) + std::abs(xx - yy);
		if (dist < minDist) {
			minDist = dist;
			closest = cable;
		}
	}
	return closest;
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