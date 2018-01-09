#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "Point.h"
#include "ProblemData.h"
#include "Object.h"


void parseArgs(int argc) {
	if (argc != 3) {
		std::cerr << "Mauvais nombre d'arguments !" << std::endl;
		exit(-1);
	}
}

int main(int argc, char *argv[]) {

	//A titre de debug pour le moment
	const char *input;
	const char *output;
	if (argc == 3) {
		input = argv[1];
		output = argv[2];
	}
	else {
		input = "charleston_road.in"; //lets_go_higher, rue_de_londres, opera
		output = "..//Output//higher67b.out";
	}

	//parseArgs(argc);
	ProblemData data;
	const char* filename(input);
	data.ParseFile(filename);

#ifdef DEBUG
	std::cout << data(0, 0) << std::endl;
	std::cout << "Nombres de points à disposition sur la carte : " << data.calculMaxMoney() << std::endl;
#endif 
#ifdef DEBUG
	std::cout << "Debut du depot des routeurs" << std::endl;
#endif

	data.depotRouter();	

#ifdef DEBUG
	long scoreCellsCovered = data.scoreRouters();
	std::cout << "On a depose " << data.getNbRouters() << " routeurs sur la carte." << std::endl;
	std::cout << "Score recuperer pour avoir convert des cellules : " << scoreCellsCovered << std::endl;
#endif 

	data.dumpInFile(output);
	return 0;
}