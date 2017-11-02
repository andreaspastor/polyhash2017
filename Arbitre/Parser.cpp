#include "Parser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

Parser::Parser() {

}

Parser::~Parser() {

}

//Parse the rule
void Parser::ParseRule(const char* filename) {
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

	file.ignore();

	unsigned int currentRow = 0;
	while (currentRow < row) {
		getline(file, line);
		map.push_back(vector<Point>());
		for (unsigned int currentCol = 0; currentCol < line.size(); currentCol++) {
			switch (line[currentCol]) {
			case '#':
				map[currentRow].push_back(Point(currentRow, currentCol, MUR));
				break;

			case '.':
				map[currentRow].push_back(Point(currentRow, currentCol, TARGET));
				break;

			case '-':
				map[currentRow].push_back(Point(currentRow, currentCol, VIDE));
				break;

			default:
				cerr << "Une erreur s'est produite lors du parsing !" << endl;
				exit(-1);
				break;
			}
		}
		currentRow += 1;
	}
	cout << map[0][0] << endl;
}

//Parse the answer file
void Parser::ParseAnswer(const char* filename) {
	ifstream file(filename);
	if (!file) {
		cerr << "This file doesn't exist !" << endl;
		exit(-1);
	}

	cout << "Opening file anwser" << endl;

	string line;
	//First line is the number of cells connected
	file >> line;
	numberOfCellsConnected = stoi(line);

	file.ignore();

	//The numberOfCellsConnected * nextLine are the cells connected
	unsigned int currentRow = 0;
	while (currentRow < numberOfCellsConnected) {
		getline(file, line);

		//Split the line (XXX XXX) with space delimiter
		vector<std::string> result;
		istringstream iss(line);
		for (std::string s; iss >> s; )
			result.push_back(s);

		//Add the point for future tests
		cells.push_back(Point(stoi(result[0]), stoi(result[1]), VIDE));
		currentRow += 1;
	}

	//Number of routers
	file >> line;
	numberOfRouters = stoi(line);

	file.ignore();

	//The numberofRouters * nextLine are the routers
	currentRow = 0;
	while (currentRow < numberOfRouters) {
		getline(file, line);

		//Split
		vector<std::string> result;
		istringstream iss(line);
		for (std::string s; iss >> s; )
			result.push_back(s);

		//Add
		routers.push_back(Point(stoi(result[0]), stoi(result[1]), VIDE));
		currentRow += 1;
	}
}

bool Parser::areRoutersConnectedToBackbone() {
	return false;
}

//Check if routers are in walls or not
//Return 1 if true, 0 otherwise
bool Parser::areRoutersInWalls() {
	//Check for each routers if his coordonnates are walls or not
	for (auto router : routers) {
		if (map[router.getCoordX()][router.getCoordY()].getType() == MUR) return false;
	}

	return true;
}

//Check if budget is respected (budged <= maxBudget)
//Return bool (0 if not respected)
bool Parser::isBudgetRespected() {
	budgetCalculated = numberOfCellsConnected * connectPrice + numberOfRouters * routerPrice;
	cout << "Budget max: " << maxBudget << " | " << "Calculated budget: " << budgetCalculated << endl;
	return budgetCalculated <= maxBudget;
}

int Parser::calculeScore() {
	return 0;
}


Point Parser::operator()(const unsigned int row, const unsigned int col)
{
	return map[row][col];
}

int Parser::getRow()
{
	return row;
}

int Parser::getCol()
{
	return col;
}

int Parser::getRouterRange()
{
	return routerRange;
}

unsigned int Parser::getConnectPrice()
{
	return connectPrice;
}

unsigned int Parser::getRouterPrice()
{
	return routerPrice;
}

unsigned int Parser::getMaxBudget()
{
	return maxBudget;
}

unsigned int Parser::getBackboneRow()
{
	return backboneRow;
}

unsigned int Parser::getBackboneCol()
{
	return backboneCol;
}