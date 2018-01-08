#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <windows.h>
#include <vector>
#include "Parser.h"
#include "FileHandling.hpp"


using namespace std;

double timeSolution(string &executable, string &input, string &strategy) {
	//Implement clock cpu time later
	double nb_test = 5;
	double total = 0;
	cout << "Lancement de " << nb_test << " tests sur => " << input << " avec => " << strategy << endl;

	for (int i = 0; i < nb_test; ++i) {

		std::clock_t startcputime = std::clock();
		system(executable.c_str());
		total += (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
		cout << i+1 << " ";
		
	}
	double result = total / nb_test;
	cout << endl << "Temps moyen: " << result << " seconde(s)" << endl;
	return result;
}

void dumpResults(string &fileName, const vector<pair<string, double>> &results) {
	
	ofstream monFlux(fileName + ".out");

	if (!monFlux) {
		cerr << "Impossible de creer le fichier : " << fileName << endl;
	}
	else {
		for (const auto &result : results) {
			monFlux << result.first << " " << result.second << " secondes" << endl;
		}
		monFlux.close();
	}
}

bool isSolutionValid(const char *rule, const char *solution) {
	Parser parser;
	parser.ParseRule((string(rule)).c_str());
	parser.ParseAnswer((string(solution)).c_str());

	if (!parser.areAllRulesRespected()) {
		cout << "L'arbitre a invalide le test, fin de l'evaluation de la solution" << endl;
		cout << parser.computeScore() << endl;
		return false;
	}
	else {
		cout << parser.computeScore() << endl;
		return true;
	}
}

void compareSolutions(const char *directory, const char *resultFile) {

	std::vector<std::string> entries = { "charleston_road.in", "rue_de_londres.in", "opera.in", "lets_go_higher.in" };

	std::string fileName;
	vector<pair<string, double>> results;

	FileHandling dirList(directory);
	int i = 0;
	while (dirList.GetNextFile(fileName))
	{
		std::string file(directory);
		file += fileName;
		std::string extension = fileName.substr(fileName.length() - 4, fileName.length());
		string dirWin = string(directory);
		std::replace(dirWin.begin(), dirWin.end(), '/', '\\');
		string path = dirWin + "\\" + fileName;

		double duration = 0;
		if (extension == ".exe") {
			for (auto entry : entries) {
				string output = fileName.substr(0, fileName.length() - 4) + "_" + entry.substr(0, entry.length()-3) + ".out";
				string command = path + " " + entry + " " + output;
				duration += timeSolution(command, entry, fileName);
				
				if (!isSolutionValid(entry.c_str(), output.c_str())) {
					duration = 0;
					break;
				}
				
			}
			results.push_back(make_pair(fileName, duration));
		}
		++i;
	}
	sort(results.begin(), results.end(), [](auto left, auto right) { return left.second < right.second; });
	dumpResults(string(resultFile), results);

}


int main(int argc, char *argv[])
{
	const char* directory;
	const char* resultFile;
	if (argc != 3) {
		//Defaut settings, only for tests
		directory = "..\\Strategies";
		resultFile = "timing_results.out";
	}
	else {
		directory = argv[1];
		resultFile = argv[2];
	}

	//isSolutionValid("charleston_road.in", "charlestonResult.txt");
	compareSolutions(directory, resultFile);


#ifdef DEBUG

	cout << "Projet ARBITRE" << endl;

	Parser parser;
	parser.ParseRule("opera.in");
	cout << "Parse rule OK" << endl;
	parser.ParseAnswer("coord.txt");
	cout << "Parse answer OK" << endl;

	cout << "Budget respected ? " << parser.isBudgetRespected() << endl;
	
	cout << "Are routers in walls ? " << parser.areRoutersInWalls() << endl;

	system("Pause");
#endif
	return 0;
}