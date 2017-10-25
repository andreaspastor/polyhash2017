#pragma once
class Point
{
public:
	Point();
	Point(int x, int y);
	unsigned int distance(Point &B);
	~Point();
	int coordX;
	int coordY;
};

