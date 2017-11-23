#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <Windows.h>
#include <vector>
#include "Parser.h"
#include "FileHandling.hpp"

/* How to use?
Compile all startegies tha you want to time and test and put the file .exe in "Strategies" directory
*/

using namespace std;

double timeSolution(string &executable, string &input, string &strategy) {
	//Implement clock cpu time later
	double nb_test = 5;
	double total = 0;
	cout << "Lancement de " << nb_test << " tests sur => " << input << " avec => " << strategy << endl;

	for (int i = 0; i < nb_test; ++i) {
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

		system(executable.c_str());

		std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
		total += sec.count();
		cout << i+1 << " ";
		
	}
	double result = total / nb_test;
	cout << endl << "Temps moyen: " << result << " seconde(s)" << endl;
	return result;
}

void dumpResults(string &fileName, const vector<pair<string, double>> &results) {
	
	ofstream monFlux("..\\Output\\" + fileName);

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
	parser.ParseRule(("..\\Input\\" + string(rule)).c_str());
	parser.ParseAnswer(("..\\Output\\" + string(solution)).c_str());

	if (!parser.areAllRulesRespected()) {
		cout << "L'arbitre a invalide le test, fin de l'evaluation de la solution" << endl;
		return false;
	}
	else {
		cout << parser.computeScore() << endl;
		return true;
	}
}

void compareSolutions(const char *directory, const char *resultFile) {

	std::vector<std::string> entries = { "rue_de_londres.in", "charleston_road.in", "opera.in", "lets_go_higher.in" };

	std::string fileName;
	vector<pair<string, double>> results;

	FileHandling dirList(directory);
	int i = 0;
	while (dirList.GetNextFile(fileName))
	{
		std::string file(directory);
		file += fileName;
		std::string extension = fileName.substr(fileName.length() - 4, fileName.length());
		string path = string(directory) + "\\" + fileName;

		//TODO: cast unix path to windows path
		double duration = 0;
		if (extension == ".exe") {
			for (auto entry : entries) {
				string output = fileName.substr(0, fileName.length() - 4) + "_" + entry;
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
		resultFile = "timing_results.txt";
	}
	else {
		directory = argv[1];
		resultFile = argv[2];
	}

	isSolutionValid("charleston_road.in", "charlestonResult.txt");
	//compareSolutions(directory, resultFile);

	system("pause");

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