#include "ProblemData.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>


ProblemData::ProblemData()
{
}


ProblemData::~ProblemData()
{
}

void ProblemData::ParseFile(const char * filename)
{
	std::ifstream file("..\\Input\\" + std::string(filename));
	if (!file) {
		std::cerr << "Ce fichier n'existe pas !" << std::endl;
		exit(-1);
	}

	std::string line;
	file >> line;
	row = std::stoi(line);
	file >> line;
	col = std::stoi(line);
	file >> line;
	routerRange = std::stoi(line);
	file >> line;
	connectPrice = std::stoi(line);
	file >> line;
	routerPrice = std::stoi(line);
	file >> line;
	maxBudget = std::stoi(line);
	file >> line;
	backboneRow = std::stoi(line);
	file >> line;
	backboneCol = std::stoi(line);

	file.ignore();
	
	unsigned int currentRow = 0;
	while (currentRow < row) {
		std::getline(file, line);
		map.push_back(std::vector<Point>());
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
				std::cerr << "Une erreur s'est produite lors du parsing !" << std::endl;
				std::exit(-1);
				break;
			}
		}
		currentRow += 1;
	}
	file.close();
#ifdef DEBUG
	std::cout << map[0][0] << std::endl;
#endif
}

void ProblemData::dumpInFile(const char * filename)
{

	std::ofstream monFlux(("..\\Output\\" + std::string(filename)));
	
	if (monFlux) {
		monFlux << cables.size() << std::endl;
		for (auto &pt : cables) {
			monFlux << pt.getCoordX() << " " << pt.getCoordY() << std::endl;
		}
		// -1 car le premier point dans la liste est le backbone
		monFlux << routers.size() - 1 << std::endl;
		for (int x = 0; x < routers.size(); x++) {
			monFlux << routers[x].getCoordX() << " " << routers[x].getCoordY() << std::endl;
		}
	}
	monFlux.close();
}

void dump(const char* filename, std::vector<Point> routers) {
	std::ofstream monFlux(filename);

	if (monFlux) {
		for (auto &router : routers) {
			monFlux << router.getCoordX() << " " << router.getCoordY() << std::endl;
		}
	}
	else {
		std::cout << "ERREUR: Impossible d'ouvrir le fichier." << std::endl;
	}

	monFlux.close();
}
//Premier solution de dépot de routers
/*vector<Point> ProblemData::depotRouter() {
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
}*/

int ProblemData::potentielWifi(int x , int y) const {
	int score = 0;
	for (int i = -routerRange; i <= routerRange; i += 4) {
		for (int j = -routerRange; j <= routerRange; j += 4) {
			if (map[x + i][y + j].getType() != COVERED && isCover(x, y, x + i, y + j)) {
				score += 1000;
			}
		}
	}
	return score;
}

int ProblemData::distance(int x, int y) const{
	int xx, yy, dist;
	int minDist = 9999;
	for (auto &cable : cables) {
		xx = std::abs(x - cable.getCoordX());
		yy = std::abs(y - cable.getCoordY());
		dist = std::fmin(xx, yy) + std::abs(xx - yy);
		if (dist < minDist) {
			minDist = dist;
		}
	}
	return minDist;
}

void ProblemData::depotRouter() {
	//ajout du backbone car besoin dans le graphe couvrant
	routers.push_back(Point(backboneRow, backboneCol, CABLE));
	cables.push_back(Point(backboneRow, backboneCol, CABLE)); //pour le parcours de la mesure de distance au depot du premier router

	Point maxPotentiel;
	Point plusProcheCable;
	int potentielValue = 1;
	int distanceToCable;
	int value = 0;

	while (maxBudget > 0) {

		//Remise à zero des valeurs
		maxPotentiel = Point(0, 0, ROUTER);
		plusProcheCable = Point(backboneRow, backboneCol, CABLE);
		potentielValue = 0;//peut etre initialiser à la valeur du coût d'un routeur
		distanceToCable = maxPotentiel.distance(plusProcheCable);

		for (int x = routerRange; x < row - routerRange; x += 5) {
			for (int y = routerRange; y < col - routerRange; y += 5) {
				value = potentielWifi(x, y) - distance(x, y);
				if (value > potentielValue) {
					maxPotentiel = Point(x, y, ROUTER);
					potentielValue = value;
				}
			}
		}
		//Ajout du router
		if (potentielValue == 0) {
			//Si l'on a pas reussi à trouver des points qui doivent prendre du wifi
			break;
		}
		routers.push_back(maxPotentiel);
		maxBudget -= routerPrice;
		
		//Modification de la map
		for (int x = -routerRange; x <= routerRange; x++) {
			for (int y = -routerRange; y <= routerRange; y++) {
				if (isCover(maxPotentiel.getCoordX(), maxPotentiel.getCoordY(), maxPotentiel.getCoordX() + x, maxPotentiel.getCoordY() + y)) {
					map[maxPotentiel.getCoordX() + x][maxPotentiel.getCoordY() + y].setType(COVERED);
				}
			}
		}
#ifdef DEBUG
		std::cout << "Ajout de routeur no " << getNbRouters() << " : " << potentielValue << std::endl;
#endif 
		//Ajout des cables
		plusProcheCable = maxPotentiel.closestCable(cables);
		std::vector<Point> linkCables = maxPotentiel.getCablesToB(plusProcheCable);
		maxBudget -= linkCables.size() * connectPrice;
		for (auto &cable : linkCables) {
			if (find(cables.begin(), cables.end(), cable) == cables.end()) {
				cables.push_back(cable);
			}
		}
#ifdef DEBUG
		std::cout << "Nombre de cables : " << getNbCables() << std::endl;
		std::cout << "Budget restant de " << maxBudget << " euros." << std::endl;
#endif 
	}
	



	/*for (int x = routerRange; x < row; x += (2 * routerRange + 1)) {
		for (int y = routerRange; y < col; y += (2 * routerRange + 1)) {
			if (map[x][y].getType() == TARGET) {
				routers.push_back(Point(x, y, ROUTER));
			}
		}
	}*/
}

std::vector<Point> ProblemData::getRepartition(const std::vector<int> & parent)
{
	Point backbone(backboneRow, backboneCol, CABLE);

	for (int x = 1; x < parent.size(); x++) {
		Point routerA = routers[x];
		Point routerB = routers[parent[x]];
		std::vector<Point> linkCables = routerA.getCablesToB(routerB);
		for (auto &cable : linkCables) {
			if (find(cables.begin(), cables.end(), cable) == cables.end()) {
				cables.push_back(cable);
			}
		}
	}
#ifdef DEBUG
	cout << listCables.size() << endl;
#endif 

	std::vector<Point> listCablesSorted;
	
	sorting(cables, listCablesSorted, backbone);
	return listCablesSorted;
}

void sorting(const std::vector<Point> & listeRef, std::vector<Point> & liste, const Point & ptCentre) 
{
	for (auto &pt : listeRef) {
		if (pt.voisinDe(ptCentre) && find(liste.begin(), liste.end(), pt) == liste.end()) {
			liste.push_back(pt);
			sorting(listeRef, liste, pt);
		}
	}
}

bool ProblemData::isCover(const Point& ptA, const Point& ptB) const
{
	int xmin = std::fmin(ptA.getCoordX(), ptB.getCoordX());
	int xmax = std::fmax(ptA.getCoordX(), ptB.getCoordX());
	int ymin = std::fmin(ptA.getCoordY(), ptB.getCoordY());
	int ymax = std::fmax(ptA.getCoordY(), ptB.getCoordY());
	for (int x = xmin; x <= xmax; x++) {
		for (int y = ymin; y <= ymax; y++) {
			if (map[x][y].getType() == MUR) {
				return false;
			}
		}
	}
	return true;
}

bool ProblemData::isCover(int ptAx, int ptAy, int ptBx, int ptBy) const
{
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

long ProblemData::scoreRouters() {
	long score = 0;
	for (auto &router : routers) {
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

long ProblemData::calculMaxMoney() const
{
	long somme = 0;
	for (auto &x: map) {
		for (auto &y : x) {
			if (y.getType() == TARGET)
			{
				somme += 1;
			}
		}
	}
	return somme * 1000;
}

std::ostream& operator<<(std::ostream& os, const ProblemData& data)
{
	os << "Lignes : " << data.row << std::endl;
	os << "Colonnes : " << data.col << std::endl;
	os << "Portee routeur : " << data.routerRange << std::endl;
	os << "Prix de connection du cable : " << data.connectPrice << std::endl;
	os << "Prix routeur : " << data.routerPrice << std::endl;
	os << "Budget : " << data.maxBudget << std::endl;
	os << "Backbone situe en " << data.backboneRow << "," << data.backboneCol << std::endl;
	return os;
}

