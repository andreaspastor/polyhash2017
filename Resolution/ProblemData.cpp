#include "ProblemData.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

ProblemData::ProblemData()
{
}


ProblemData::~ProblemData()
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

	file.ignore();
	
	unsigned int currentRow = 0;
	while (currentRow < row) {
		getline(file, line);
		map.push_back(vector<Point>());
		for (unsigned int currentCol = 0; currentCol < line.size(); currentCol ++) {
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

void ProblemData::dumpInFile(const char * filename, vector<Point>& routers, const vector<Point>& listeCables)
{
	ofstream monFlux(filename);
	
	if (monFlux) {
		monFlux << listeCables.size() << endl;
		for (auto pt : listeCables) {
			monFlux << pt.getCoordX() << " " << pt.getCoordY() << endl;
		}
		// -1 car le premier point dans la liste est le backbone
		monFlux << routers.size() - 1 << endl;
		for (int x = 0; x < routers.size(); x++) {
			monFlux << routers[x].getCoordX() << " " << routers[x].getCoordY() << endl;
		}
	}
}

void dump(const char* filename, vector<Point> routers) {
	ofstream monFlux(filename);

	if (monFlux) {
		for (auto router : routers) {
			monFlux << router.getCoordX() << " " << router.getCoordY() << endl;
		}
	}
	else {
		cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}
}

vector<Point> ProblemData::depotRouter() {
	vector<Point> routers;
	//ajout du backbone car besoin dans le graphe couvrant
	routers.push_back(Point(backboneRow, backboneCol, CABLE));

	for (int x = routerRange; x < row; x += (2 * routerRange + 1)) {
		for (int y = routerRange; y < col; y += (2 * routerRange + 1)) {
			if (map[x][y].getType() == TARGET) {
				routers.push_back(Point(x, y, ROUTER));
			}
		}
	}
	return routers;
}

vector<Point> ProblemData::getRepartition(vector<Point> & routers, const vector<int> & parent)
{
	vector<Point> listCables;
	Point backbone(backboneRow, backboneCol, CABLE);

	for (int x = 1; x < parent.size(); x++) {
		Point routerA = routers[x];
		Point routerB = routers[parent[x]];
		vector<Point> linkCables = routerA.getCablesToB(routerB);
		for (auto cable : linkCables) {
			if (find(listCables.begin(), listCables.end(), cable) == listCables.end()) {
				listCables.push_back(cable);
			}
		}
	}

	cout << listCables.size() << endl;
	vector<Point> listCablesSorted;
	
	sorting(listCables, listCablesSorted, backbone);
	return listCablesSorted;
}

void sorting(const vector<Point> & listeRef, vector<Point> & liste, const Point & ptCentre) 
{
	for (auto pt : listeRef) {
		if (pt.voisinDe(ptCentre) && find(liste.begin(), liste.end(), pt) == liste.end()) {
			liste.push_back(pt);
			sorting(listeRef, liste, pt);
		}

	}
}

bool ProblemData::isCover(Point& ptA, Point& ptB)
{
	int xmin = fmin(ptA.getCoordX(), ptB.getCoordX());
	int xmax = fmax(ptA.getCoordX(), ptB.getCoordX());
	int ymin = fmin(ptA.getCoordY(), ptB.getCoordY());
	int ymax = fmax(ptA.getCoordY(), ptB.getCoordY());
	for (int x = xmin; x <= xmax; x++) {
		for (int y = ymin; y <= ymax; y++) {
			if (map[x][y].getType() == MUR) {
				return false;
			}
		}
	}
	return true;
}

long ProblemData::scoreRouters(const vector<Point>& routers) {
	long score = 0;
	for (auto router : routers) {
		for (int x = -routerRange; x <= routerRange; x++) {
			for (int y = -routerRange; y <= routerRange; y++) {
				if (map[router.getCoordX() + x] [router.getCoordY() + y].getType() != COVERED && isCover(router, map[router.getCoordX() + x][router.getCoordY() + y])) {
					score += 1000;
					map[router.getCoordX() + x][router.getCoordY() + y].setType(COVERED);//Besoin de dire COVERED pour eviter de compter 2 fois la meme cellule
				}
			}
		}
	}
	return score;
}

long ProblemData::calculMaxMoney()
{
	long somme = 0;
	for (auto x: map) {
		for (auto y : x) {
			if (y.getType() == TARGET)
			{
				somme += 1;
			}
		}
	}
	return somme * 1000;
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

Point ProblemData::operator()(const unsigned int row, const unsigned int col)
{
	return map[row][col];
}

int ProblemData::getRow()
{
	return row;
}

int ProblemData::getCol()
{
	return col;
}

int ProblemData::getRouterRange()
{
	return routerRange;
}

unsigned int ProblemData::getConnectPrice()
{
	return connectPrice;
}

unsigned int ProblemData::getRouterPrice()
{
	return routerPrice;
}

unsigned int ProblemData::getMaxBudget()
{
	return maxBudget;
}

unsigned int ProblemData::getBackboneRow()
{
	return backboneRow;
}

unsigned int ProblemData::getBackboneCol()
{
	return backboneCol;
}
