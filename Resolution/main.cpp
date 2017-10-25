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
vector<Point> depotRouter(ProblemData& data);
long scoreRouters(ProblemData& data, vector<Point>& routers);
void dump(const char* filename, vector<Point> routers);

void dump(const char* filename, vector<Point> routers) {
	ofstream monFlux(filename);

	if (monFlux) {
		for (auto router : routers) {
			monFlux << router.getCoordX() << " " << router.getCoordY() << endl;
		}
	}
	else {
		cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}
}

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

vector<Point> depotRouter(ProblemData& data) {
	vector<Point> routeurs;
	for (int x = data.getRouterRange(); x < data.getRow(); x += (2 * data.getRouterRange() + 1)) {
		for (int y = data.getRouterRange(); y < data.getCol(); y += (2 * data.getRouterRange() + 1)) {
			if (data(x, y).getType() == TARGET) {
				routeurs.push_back(Point(x, y, ROUTER));
			}
		}
	}
	cout << routeurs.size() << endl;
	return routeurs;
}

long scoreRouters(ProblemData& data, vector<Point>& routers) {
	long score = 0;
	for (auto router : routers) {
		for (int x = -data.getRouterRange(); x <= data.getRouterRange(); x++) {
			for (int y = -data.getRouterRange(); y <= data.getRouterRange(); y++) {
				if (data(router.getCoordX() + x, router.getCoordY() + y).getType() != COVERED && isCover(data, router, data(router.getCoordX() + x, router.getCoordY() + y))) {
					score += 1000;
					data(router.getCoordX() + x, router.getCoordY() + y).setType(COVERED);//Besoin de dire COVERED pour eviter de compter 2 fois la meme cellule
				}
			}
		}
	}
	return score;
}

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
	data.ParseFile("rue_de_londres.in");
	cout << data(0, 0) << endl;
	cout << "Nombres de points à disposition sur la carte : " << data.calculMaxMoney() << endl;

	vector<Point> routers = depotRouter(data);
	long scoreCellsCovered = scoreRouters(data, routers);

	cout << "On a depose " << routers.size() << " routeurs sur la carte." << endl;
	cout << "Score recuperer pour avoir convert des cellules : " << scoreCellsCovered << endl;

	dump("coordRouters.txt", routers);

	Point ptA(12, 12, MUR);
	Point ptB(5, 5, VIDE);
	cout << "Distance entre les points : " << ptA.distance(ptB) << endl;
	data.Resolution();
	return 0;
}