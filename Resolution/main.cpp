#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ProblemData.h"

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

	data.Resolution();
	return 0;
}