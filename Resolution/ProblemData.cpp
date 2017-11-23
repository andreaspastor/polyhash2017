#include "ProblemData.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

#define DEBUG

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
		mapEntree.push_back(std::vector<Point>());
		mapSortie.push_back(std::vector<Point>());
		for (unsigned int currentCol = 0; currentCol < line.size(); currentCol ++) {
			switch (line[currentCol]) {
			case '#':
				mapEntree[currentRow].push_back(Point(currentRow, currentCol, MUR));
				mapSortie[currentRow].push_back(Point(currentRow, currentCol, MUR));
				break;

			case '.':
				mapEntree[currentRow].push_back(Point(currentRow, currentCol, TARGET));
				mapSortie[currentRow].push_back(Point(currentRow, currentCol, TARGET));
				break;

			case '-':
				mapEntree[currentRow].push_back(Point(currentRow, currentCol, VIDE));
				mapSortie[currentRow].push_back(Point(currentRow, currentCol, VIDE));
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
	std::cout << mapEntree[0][0] << std::endl;
#endif
}

void ProblemData::dumpInFile(const char * filename)
{

	std::ofstream monFlux(("..\\Output\\" + std::string(filename)));
	
	if (monFlux) {
		monFlux << cablesSorted.size() << std::endl;
		for (auto &pt : cablesSorted) {
			monFlux << pt.getCoordX() << " " << pt.getCoordY() << std::endl;
		}
		monFlux << routers.size() << std::endl;
		for (auto &router : routers) {
			monFlux << router.getCoordX() << " " << router.getCoordY() << std::endl;
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
			if (mapEntree[x][y].getType() == TARGET) {
				routers.push_back(Point(x, y, ROUTER));
			}
		}
	}
	return routers;
}*/

int ProblemData::potentielWifi(int x , int y) const {
	int score = 0;
	for (int i = -routerRange; i <= routerRange; i += 1) {
		for (int j = -routerRange; j <= routerRange; j += 1) {
			if (x + i > -1 && y + j > -1 && x + i < row && y + j < col) {
				if (mapEntree[x + i][y + j].getType() == TARGET && mapSortie[x + i][y + j].getType() != COVERED && isCover(x, y, x + i, y + j)) {
					score += 1000;
				}
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

int ProblemData::distanceNewCables(int x, int y, const std::vector<Point> & newCables) const {
	int xx, yy, dist;
	int minDist = 9999;
	for (auto &cable : newCables) {
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

	//initialisation mapRecherche
	for (int x = 0; x < row; x++) {
		mapSearchCab.push_back(std::vector<double>());
		mapSearchCov.push_back(std::vector<double>());
		std::cout << x << " / " << row << std::endl;
		for (int y = 0; y < col; y++) {
			mapSearchCov[x].push_back(potentielWifi(x, y));
			mapSearchCab[x].push_back(distance(x, y));
		}
	}

	while (maxBudget > 0) {

		//Remise à zero des valeurs
		maxPotentiel = Point(0, 0, ROUTER);
		plusProcheCable = Point(backboneRow, backboneCol, CABLE);
		potentielValue = 0;//peut etre initialiser à la valeur du coût d'un routeur
		distanceToCable = maxPotentiel.distance(plusProcheCable);

		for (int x = 0; x < mapSearchCov.size(); x++) {
			for (int y = 0; y < mapSearchCov[x].size(); y++) {
				value = mapSearchCov[x][y] - mapSearchCab[x][y];
				if (value > potentielValue) {
					maxPotentiel = Point(x, y, ROUTER);
					potentielValue = value;
				}
			}
		}
		/*for (int x = routerRange; x < row - routerRange; x += 5) {
			for (int y = routerRange; y < col - routerRange; y += 5) {
				value = potentielWifi(x, y) - distance(x, y);
				if (value > potentielValue) {
					maxPotentiel = Point(x, y, ROUTER);
					potentielValue = value;
				}
			}
		}*/

		//Ajout du router
		if (potentielValue == 0) {
			//Si l'on a pas reussi à trouver des points qui doivent prendre du wifi
			break;
		}
		plusProcheCable = maxPotentiel.closestCable(cables);
		std::vector<Point> linkCables = maxPotentiel.getCablesToB(plusProcheCable);
		linkCables.push_back(Point(maxPotentiel.getCoordX(), maxPotentiel.getCoordY(), CABLE));
		maxBudget -= linkCables.size() * connectPrice;
		
		maxBudget -= routerPrice;
		
		if (maxBudget < 0) {
			break;
		}
		routers.push_back(maxPotentiel);

		//Modification de la map
		for (int x = -routerRange; x <= routerRange; x++) {
			for (int y = -routerRange; y <= routerRange; y++) {
				if (isCover(maxPotentiel.getCoordX(), maxPotentiel.getCoordY(), maxPotentiel.getCoordX() + x, maxPotentiel.getCoordY() + y)) {
					mapSortie[maxPotentiel.getCoordX() + x][maxPotentiel.getCoordY() + y].setType(COVERED);
				}
			}
		}

		//update de la map de solution couverture wifi après avoir update la couverture
		for (int x = -2*routerRange; x <= 2*routerRange; x++) {
			for (int y = -2*routerRange; y <= 2*routerRange; y++) {
				mapSearchCov[maxPotentiel.getCoordX() + x][maxPotentiel.getCoordY() + y] = potentielWifi(maxPotentiel.getCoordX() + x, maxPotentiel.getCoordY() + y);
			}
		}
#ifdef DEBUG
		std::cout << "Ajout de routeur no " << getNbRouters() << " : " << potentielValue << std::endl;
#endif 
		//Ajout des cables
		std::vector<Point> newCables;
		for (auto &cable : linkCables) {
			if (find(cables.begin(), cables.end(), cable) == cables.end()) {//newCable UNIQUE !!!
				cables.push_back(cable);
				newCables.push_back(cable);
			}
		}

		//update de la map de solution cable apres que l'on est mis les cables à jour
		for (int x = 0; x < mapSearchCab.size(); x++) {
			for (int y = 0; y < mapSearchCab[x].size(); y++) {
				value = distanceNewCables(x, y, newCables);//amélioration en passant seulement newCable UNIQUE !!!
				if (value < mapSearchCab[x][y]) {
					mapSearchCab[x][y] = value;
				}
			}
		}
#ifdef DEBUG
		std::cout << "Nombre de cables : " << getNbCables() << std::endl;
		std::cout << "Budget restant de " << maxBudget << " euros." << std::endl;
#endif 
	}
	
	

	std::cout << "Sorting des cables" << std::endl;
	Point backbone(backboneRow, backboneCol, CABLE);
	//suppression du point backbone avant de sort, il avait été utile pour trouver le plus court distance router cable
	cables.erase(cables.begin());
	sorting(cables, cablesSorted, backbone);

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
	std::cout << cables.size() << std::endl;
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
			if (mapEntree[x][y].getType() == MUR) {
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
			if (mapEntree[x][y].getType() == MUR) {
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
				if (mapSortie[router.getCoordX() + x] [router.getCoordY() + y].getType() != COVERED && isCover(router, mapEntree[router.getCoordX() + x][router.getCoordY() + y])) {
					score += 1000;
					mapSortie[router.getCoordX() + x][router.getCoordY() + y].setType(COVERED);//Besoin de dire COVERED pour eviter de compter 2 fois la meme cellule
				}
			}
		}
	}
	return score;
}

long ProblemData::calculMaxMoney() const
{
	long somme = 0;
	for (auto &x: mapEntree) {
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

