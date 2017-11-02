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
	if (argc != 3) {
		cerr << "Mauvais nombre d'arguments !" << endl;
		exit(-1);
	}
}

int main(int argc, char *argv[])
{

	//A titre de debug pour le moment
	const char *input;
	const char *output;
	if (argc == 3) {
		input = argv[1];
		output = argv[2];
	}
	else {
		input = "opera.in";
		output = "coord.txt";
	}

	//parseArgs(argc);
	ProblemData data;
	const char* filename(input);
	data.ParseFile(filename);
#ifdef DEBUG
	cout << data(0, 0) << endl;
	cout << "Nombres de points à disposition sur la carte : " << data.calculMaxMoney() << endl;
#endif 
	

	vector<Point> routers = data.depotRouter();
	long scoreCellsCovered = data.scoreRouters(routers);

#ifdef DEBUG
	cout << "On a depose " << routers.size() << " routeurs sur la carte." << endl;
	cout << "Score recuperer pour avoir convert des cellules : " << scoreCellsCovered << endl;
#endif 
	
	/*Point ptA = Point(0, 1, TARGET);
	Point ptB = Point(5, -10, TARGET);
	vector<Point> liste = ptA.getCablesToB(ptB);
	for (int f = 0; f < liste.size(); f++) {
		cout << liste[f].getCoordX() << " - " << liste[f].getCoordY() << endl;
	}*/
	
	Graph cables = Graph(routers);
	cables.resolve();
	
	vector<Point> listCables = data.getRepartition(routers, cables.parent);
	data.dumpInFile(output, routers, listCables);
	return 0;
}