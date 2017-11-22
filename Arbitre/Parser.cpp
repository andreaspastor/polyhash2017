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
	file.close();
}

//Parse the answer file
void Parser::ParseAnswer(const char* filename) {
	ifstream file(filename);
	if (!file) {
		cerr << "This file doesn't exist !" << endl;
		exit(-1);
	}
#ifdef DEBUG
	cout << "Opening file anwser" << endl;
#endif
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
	file.close();
}

void Parser::initialiseMapSolution()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			setPointMapSolution(Point(i, j, VIDE));
		}
	}
}

Point & Parser::getPointMapSolution(unsigned int x, unsigned int y)
{
	return map_solution[std::pair<unsigned int, unsigned int>(x,y)];
}

void Parser::setPointMapSolution(const Point & p)
{
	map_solution[std::make_pair(p.getCoordX(), p.getCoordY())] = p;
}

bool Parser::areRoutersConnectedToBackbone(){
	bool founded = false;
	initialiseMapSolution();
	getPointMapSolution(getBackboneRow(),getBackboneCol()).setType(CABLE);
	for (auto& fil : cells){
		//Pour chaque cable, on parcours ses 8 voisins
		//Et on vérifie qu'au moins 1 d'entre eux est un cable
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (getPointMapSolution(fil.getCoordX()+i, fil.getCoordY()+j).getType() == CABLE) {
					//Si un de ses voisins est un cable, on l'ajoute dans la map solution
					founded = true;
					getPointMapSolution(fil.getCoordX()+i, fil.getCoordY()+j).setType(CABLE);
					break;
				}
			}
			if (founded) {
				break;
			}
		}
		if (!founded) {
			return false;
		}
		founded = false;
	}

	for (auto &router : routers) {
		if (getPointMapSolution(router.getCoordX(), router.getCoordY()).getType() != CABLE) {
			return false;
		}
	}

	return true;
}

//Check if routers are in walls or not
//Return 1 if true, 0 otherwise
bool Parser::areRoutersInWalls() const {
	//Check for each routers if his coordonnates are walls or not
	for (auto &router : routers) {
		if (map[router.getCoordX()][router.getCoordY()].getType() == MUR) return false;
	}

	return true;
}

//Check if budget is respected (budged <= maxBudget)
//Return bool (0 if not respected)
bool Parser::isBudgetRespected() const {
	int budgetCalculated = numberOfCellsConnected * connectPrice + numberOfRouters * routerPrice;
#ifdef DEBUG
	cout << "Budget max: " << maxBudget << " | " << "Calculated budget: " << budgetCalculated << endl;
#endif
	return budgetCalculated <= maxBudget;
}

bool Parser::areAllRulesRespected(){
	return (isBudgetRespected() && !areRoutersInWalls() && areRoutersConnectedToBackbone());
}

bool Parser::isRouterCoveringCell(int ptAx, int ptAy, int ptBx, int ptBy) const{
	int xmin = fmin(ptAx, ptBx);
	int xmax = fmax(ptAx, ptBx);
	int ymin = fmin(ptAy, ptBy);
	int ymax = fmax(ptAy, ptBy);
	for (int x = xmin; x <= xmax; x++) {
		for (int y = ymin; y <= ymax; y++) {
			if (map[x][y].getType() == MUR) {
				return false;
			}
		}
	}
	return true;
}

void Parser::coverCellsMap(){
	for (auto &router : routers) {
		for (int x = -routerRange; x <= routerRange; x++) {
			for (int y = -routerRange; y <= routerRange; y++) {
				if (isRouterCoveringCell(router.getCoordX(), router.getCoordY(), router.getCoordX() + x, router.getCoordY() + y)) {
					map[router.getCoordX() + x][router.getCoordY() + y].setType(COVERED);
				}
			}
		}
	}
}

int Parser::computeScore() const {
	return 0;
}
