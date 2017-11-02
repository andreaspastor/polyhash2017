#include <iostream>
#include <string>
#include <chrono>
#include <Windows.h>
#include "Parser.h"
#include "FileHandling.hpp"

using namespace std;

void performTest(string fileName, string resultFile) {
	cout << fileName << endl;

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	system("..\\x64\\Release\\Resolution.exe");

	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	std::cout << "took " << sec.count() << " seconds\n";
}


int main(int argc, char *argv[])
{
	const char* directory;
	const char* resultFile;
	if (argc != 3) {
		//Default settings if wrong number of arguments
		directory = "..\\Strategies";
		resultFile = "results.txt";
	}
	else {
		directory = argv[1];
		resultFile = argv[2];
	}
	
	IterateOnFileDir(directory, resultFile, performTest);

	system("PAUSE");

	cout << "Projet ARBITRE" << endl;

	Parser parser;
	parser.ParseRule("opera.in");
	cout << "Parse rule OK" << endl;
	parser.ParseAnswer("coord.txt");
	cout << "Parse answer OK" << endl;

	cout << "Budget respected ? " << parser.isBudgetRespected() << endl;
	
	cout << "Are routers in walls ? " << parser.areRoutersInWalls() << endl;

	system("Pause");

	return 0;
}