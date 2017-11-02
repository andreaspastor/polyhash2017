#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <Windows.h>
#include "Parser.h"
#include "FileHandling.hpp"

using namespace std;

double timeSolution(string executable) {
	//Implement clock cpu time later
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	system(executable.c_str());

	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	return sec.count();
}


int main(int argc, char *argv[])
{
	const char* directory;
	const char* resultFile;
	if (argc != 3) {
		//Defaut settings, only for tests
		directory = "..\\Strategies";
		resultFile = "results.txt";
	}
	else {
		directory = argv[1];
		resultFile = argv[2];
	}
	
	std::vector<std::string> entries = { "lets_go_higher.in", "rue_de_londres.in", "charleston_road.in", "opera.in" };

	std::string fileName;

	FileHandling dirList(directory);
	int i = 0;
	while (dirList.GetNextFile(fileName))
	{
		std::string file(directory);
		file += fileName;
		std::string extension = fileName.substr(fileName.length() - 4, fileName.length());
		string path = string(directory) + "\\" + fileName;
		
		//TODO: Temps moyen

		//TODO: cast unix path to windows path
		if (extension == ".exe") {
			for (auto entry : entries) {
				string output = fileName.substr(0, fileName.length() - 4) + "_" + entry;
				string command = path + " " + entry + " " + output;
				double duration = timeSolution(command);
			}
		}

		//Check validy of the solution

		

		++i;
	}



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