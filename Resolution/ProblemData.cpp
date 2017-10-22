#include "ProblemData.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

ProblemData::ProblemData()
{
}


ProblemData::~ProblemData()
{
}

void ProblemData::Resolution()
{
	
}

void ProblemData::ParseFile(const char * filename)
{
	ifstream file(filename);
	if (!file) {
		cerr << "Ce fichier n'existe pas !" << endl;
		exit(-1);
	}

	string line;
	file >> line;
	row = stoi(line);
	file >> line;
	col = stoi(line);
	file >> line;
	routerRange = stoi(line);
	file >> line;
	connectPrice = stoi(line);
	file >> line;
	routerPrice = stoi(line);
	file >> line;
	maxBudget = stoi(line);
	file >> line;
	backboneRow = stoi(line);
	file >> line;
	backboneCol = stoi(line);

	
	unsigned int currentRow = 0;
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

	map[backboneRow][backboneCol] = BACKBONE;
}

ostream& operator<<(ostream& os, const ProblemData& data)
{
	os << "Lignes : " << data.row << endl;
	os << "Colonnes : " << data.col << endl;
	os << "Portee routeur : " << data.routerRange << endl;
	os << "Prix de connection du cable : " << data.connectPrice << endl;
	os << "Prix routeur : " << data.routerPrice << endl;
	os << "Budget : " << data.maxBudget << endl;
	os << "Backbone situe en " << data.backboneRow << "," << data.backboneCol << endl;
	return os;
}

Object ProblemData::operator()(const unsigned int row, const unsigned int col)
{
	return map.at(row).at(col);
}
