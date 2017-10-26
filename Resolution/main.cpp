#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "Point.h"
#include "ProblemData.h"
#include "Object.h"
#include "Graph.h"

using namespace std;

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
	cout << data(0, 0) << endl;
	cout << "Nombres de points à disposition sur la carte : " << data.calculMaxMoney() << endl;

	vector<Point> routers = data.depotRouter();
	long scoreCellsCovered = data.scoreRouters(routers);

	cout << "On a depose " << routers.size() << " routeurs sur la carte." << endl;
	cout << "Score recuperer pour avoir convert des cellules : " << scoreCellsCovered << endl;

	dump("coordRouters.txt", routers);

	routers.push_back(Point(data.getBackboneRow(), data.getBachboneCol(), CABLE));
	Graph cables = Graph(routers);
	cables.resolve();
	
	vector<Point> listCables = cables.getRepartition(data);
	return 0;
}