#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
enum Object { MUR, TARGET, VIDE, BACKBONE };

void parseArgs(int argc)
{
	if (argc != 2) {
		cerr << "Mauvais nombre d'arguments !" << endl;
		exit(-1);
	}
}

vector<vector<Object>> readFile(const string & name)
{
	ifstream file(name);
	if (!file) {
		cerr << "Ce fichier n'existe pas !" << endl;
	}

	string line;
	file >> line;
	int row = stoi(line);
	file >> line;
	int col = stoi(line);
	file >> line;
	int rangeRouter = stoi(line);
	file >> line;
	int priceConnect = stoi(line);
	file >> line;
	int priceRouter = stoi(line);
	file >> line;
	int maxBudget = stoi(line);
	file >> line;
	int tempBackboneRow = stoi(line);
	file >> line;
	int tempBackBoneCol = stoi(line);

	cout << "Lignes : " << row << endl;
	cout << "Colonnes : " << col << endl;
	cout << "Portée routeur : " << rangeRouter << endl;
	cout << "Prix de connection du cable : " << priceConnect << endl;
	cout << "Prix routeur : " << priceRouter << endl;
	cout << "Budget : " << maxBudget << endl;
	cout << "Backbone situé en " << tempBackboneRow << "," << tempBackBoneCol << endl;
	vector<vector<Object>> map;
	int currentRow = 0;
	while (currentRow < row) {
		getline(file, line);
		map.push_back(vector<Object>());
		for (unsigned int currentCol = 0; currentCol < line.size(); currentCol += 1) {
			switch (line[currentCol]) {
			case '#':
				map[currentRow].push_back(MUR);
				break;

			case '.':
				map[currentRow].push_back(TARGET);
				break;

			case '-':
				map[currentRow].push_back(VIDE);
				break;

			default:
				cerr << "Une erreur s'est produite lors du parsing !" << endl;
				break;
			}
		}
		currentRow += 1;
	}

	map[tempBackboneRow][tempBackBoneCol] = BACKBONE;
	return map;
}

int main(int argc, char *argv[])
{
	parseArgs(argc);
	readFile(argv[1]);
	return 0;
}