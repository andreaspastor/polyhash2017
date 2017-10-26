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
	const char* filename("opera.in");
	data.ParseFile("opera.in");
	cout << data(0, 0) << endl;
	cout << "Nombres de points à disposition sur la carte : " << data.calculMaxMoney() << endl;

	vector<Point> routers = data.depotRouter();
	long scoreCellsCovered = data.scoreRouters(routers);

	cout << "On a depose " << routers.size() << " routeurs sur la carte." << endl;
	cout << "Score recuperer pour avoir convert des cellules : " << scoreCellsCovered << endl;

	
	/*Point ptA = Point(0, 1, TARGET);
	Point ptB = Point(5, -10, TARGET);
	vector<Point> liste = ptA.getCablesToB(ptB);
	for (int f = 0; f < liste.size(); f++) {
		cout << liste[f].getCoordX() << " - " << liste[f].getCoordY() << endl;
	}*/
	
	Graph cables = Graph(routers);
	cables.resolve();
	
	vector<Point> listCables = data.getRepartition(routers, cables.parent);
	data.dumpInFile("coord.txt", routers, listCables);
	return 0;
}