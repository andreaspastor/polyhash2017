#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Point.h"
#include "ProblemData.h"
#include "Object.h"

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