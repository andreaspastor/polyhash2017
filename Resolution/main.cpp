#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "Point.h"
#include "ProblemData.h"
#include "Object.h"

using namespace std;

bool isCover(ProblemData& data, Point& ptA, Point& ptB);
//void depotRouter(ProblemData& data);

bool isCover(ProblemData& data, Point& ptA, Point& ptB) 
{
	int xmin = fmin(ptA.getCoordX(), ptB.getCoordX());
	int xmax = fmax(ptA.getCoordX(), ptB.getCoordX());
	int ymin = fmin(ptA.getCoordY(), ptB.getCoordY());
	int ymax = fmax(ptA.getCoordY(), ptB.getCoordY());
	for (int x = xmin; x <= xmax; x++) {
		for (int y = ymin; y <= ymax; y++) {
			if (data(x, y).getType() == MUR) {
				return false;
			}
		}
	}
	return true;
}

/*void depotRouter(ProblemData& data) {
	for (int x = data.getRange(); x < data.getRow(); )
}*/

void parseArgs(int argc)
{
	if (argc != 2) {
		cerr << "Mauvais nombre d'arguments !" << endl;
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	//parseArgs(argc);
	ProblemData data;
	data.ParseFile("opera.in");
	cout << data << endl;
	cout << data(333, 270) << endl;
	cout << "Nombres de points à disposition sur la carte : " << data.calculMaxMoney() << endl;

	Point ptA(12, 12, MUR);
	Point ptB(5, 5, VIDE);
	cout << "Distance entre les points : " << ptA.distance(ptB) << endl;
	data.Resolution();
	return 0;
}