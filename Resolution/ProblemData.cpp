#include "ProblemData.h"
#include "Point.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <set>
#include <cassert>

//Constructeur et destructeur de notre classe contenant la problème et sa résolution
ProblemData::ProblemData(){}
ProblemData::~ProblemData(){}

//Fonction permettant de parser les fichiers d'entrée données
void ProblemData::ParseFile(const char * filename) {
	std::ifstream file(filename);
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
	maxBudgetInit = maxBudget;
	file >> line;
	backboneRow = std::stoi(line);
	Point::backboneRow = backboneRow;
	file >> line;
	backboneCol = std::stoi(line);
	Point::backboneCol = backboneCol;

	//Afin de changer de mode de parsing entre 2 lignes
	file.ignore();

	unsigned int currentRow = 0;
	while (currentRow < row) {
		std::getline(file, line);
		mapEntree.push_back(std::vector<Point>());
		mapSortie.push_back(std::vector<Point>());
		for (unsigned int currentCol = 0; currentCol < line.size(); currentCol++) {
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

//Fonction permettant de pouvoir sauvegarder les résultats de la résolution
void ProblemData::dumpInFile(const char * filename) {

	std::ofstream monFlux(filename);

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
	else {
		std::cout << "ERREUR: Impossible d'ouvrir le fichier." << std::endl;
	}
	monFlux.close();
}

//Fonction permettant de pouvoir sauvegarder les résultats de la résolution
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

//Fonction calculant le potentiel de wifi dans une zone si on y place un routeur
long ProblemData::potentielWifi(int x, int y) const {
	long score = 0;
	if (x == backboneRow && y == backboneRow) {
		return 0;
	}

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

//Fonction pour calculer la plus courte distance entre les coordonnées et le cable le plus proche
int ProblemData::distance(int x, int y) const {
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

//Fonction pour calculer la plus courte distance entre une coordonnée et les éléments d'une liste
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

//Fonction qui permet en utilisant notre stratégie de déposer les routeurs
void ProblemData::depotRouter() {
	cables.push_back(Point(backboneRow, backboneCol, CABLE)); //pour le parcours de la mesure de distance au depot du premier router

	Point maxPotentiel;
	Point plusProcheCable;
	int potentielValue = 1;
	int distanceToCable;
	int value = 0;

	std::set<Point, CompPoint> routerSorted;

	//initialisation mapRecherche
	for (int x = 0; x < row; x++) {
		mapSearchCab.push_back(std::vector<double>());
		mapSearchCov.push_back(std::vector<double>());
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
				if (value > potentielValue && x != backboneRow && y != backboneCol) {
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
		plusProcheCable = maxPotentiel.closestCable(cables);
		//std::vector<Point> linkCables = maxPotentiel.getCablesToB(plusProcheCable);
		std::vector<Point> linkCables = maxPotentiel.getCablesDiagTo(plusProcheCable);
		linkCables.push_back(Point(maxPotentiel.getCoordX(), maxPotentiel.getCoordY(), CABLE));
		maxBudget -= linkCables.size() * connectPrice;

		maxBudget -= routerPrice;

		if (maxBudget < 0) {
			break;
		}
		routers.push_back(maxPotentiel);
		auto ret = routerSorted.insert(maxPotentiel);
		if (ret.second == false) {
			std::cout << maxPotentiel << std::endl;
		}

		//Modification de la map
		for (int x = -routerRange; x <= routerRange; x++) {
			for (int y = -routerRange; y <= routerRange; y++) {
				if (isCover(maxPotentiel.getCoordX(), maxPotentiel.getCoordY(), maxPotentiel.getCoordX() + x, maxPotentiel.getCoordY() + y)) {
					mapSortie[maxPotentiel.getCoordX() + x][maxPotentiel.getCoordY() + y].setType(COVERED);
				}
			}
		}

		//update de la map de solution couverture wifi après avoir update la couverture
		for (int x = -2 * routerRange; x <= 2 * routerRange; x++) {
			for (int y = -2 * routerRange; y <= 2 * routerRange; y++) {
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

	// drop des cables 
	cables.clear();
	std::vector<Point> routersCopy = routers;
	std::vector<Point> routersCopy2;
	//Recablage a la fin
	Point backbonePt = Point(backboneRow, backboneCol, CABLE);
	Point nextRouter = backbonePt.closestCable(routersCopy);
	std::vector<Point> link = backbonePt.getCablesDiagTo(nextRouter);
	link.push_back(Point(nextRouter.getCoordX(), nextRouter.getCoordY(), CABLE));//pas besoin d'ajouter le backbone dans la liste
	for (auto &cable : link) {
		if (find(cables.begin(), cables.end(), cable) == cables.end()) {//newCable UNIQUE !!!
			cables.push_back(cable);
		}
	}
	Point oldRouter = nextRouter;
	Point routerA = backbonePt;
	Point routerB = nextRouter;
	routersCopy2.push_back(backbonePt);
	routersCopy2.push_back(nextRouter);
	routersCopy.erase(find(routersCopy.begin(), routersCopy.end(), nextRouter));
	int minDist;
	while (routersCopy.size() > 0) {
		minDist = 9999;
		std::cout << routersCopy.size() << std::endl;
		for (auto & router : cables) {
			nextRouter = router.closestCable(routersCopy);//prochain router à cabler
			if (nextRouter.distance(router) < minDist) {
				minDist = nextRouter.distance(router);
				routerB = nextRouter;
				routerA = router;
			}
		}
		routersCopy2.push_back(routerB);
		plusProcheCable = routerB.closestCable(cables);//cable ou router le plus proche pour le cablage
		std::vector<Point> link = routerB.getCablesDiagTo(plusProcheCable);//calcul des cables à ajouter
		link.push_back(Point(routerB.getCoordX(), routerB.getCoordY(), CABLE));// a commenter pour voir l'effet
		for (auto &cable : link) {
			if (find(cables.begin(), cables.end(), cable) == cables.end()) {//newCable UNIQUE !!!
				cables.push_back(cable);
			}
		}
		routersCopy.erase(find(routersCopy.begin(), routersCopy.end(), routerB));//suppression du router cablé
	}


	int budgetRestant = maxBudgetInit - routers.size() * routerPrice - cables.size() * connectPrice;
	std::cout << "BUDGET RESTANT : " << budgetRestant << std::endl;
	maxBudget = budgetRestant;

	// DEBUT CC
	while (maxBudget > 0) {

		//Remise à zero des valeurs
		maxPotentiel = Point(0, 0, ROUTER);
		plusProcheCable = Point(backboneRow, backboneCol, CABLE);
		potentielValue = 0;//peut etre initialiser à la valeur du coût d'un routeur
		distanceToCable = maxPotentiel.distance(plusProcheCable);

		for (int x = 0; x < mapSearchCov.size(); x++) {
			for (int y = 0; y < mapSearchCov[x].size(); y++) {
				value = mapSearchCov[x][y] - mapSearchCab[x][y];
				if (value > potentielValue && x != backboneRow && y != backboneCol) {
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
		plusProcheCable = maxPotentiel.closestCable(cables);
		//std::vector<Point> linkCables = maxPotentiel.getCablesToB(plusProcheCable);
		std::vector<Point> linkCables = maxPotentiel.getCablesDiagTo(plusProcheCable);
		linkCables.push_back(Point(maxPotentiel.getCoordX(), maxPotentiel.getCoordY(), CABLE));
		maxBudget -= linkCables.size() * connectPrice;

		maxBudget -= routerPrice;

		if (maxBudget < 0) {
			break;
		}
		routers.push_back(maxPotentiel);
		routerSorted.insert(maxPotentiel);

		//Modification de la map
		for (int x = -routerRange; x <= routerRange; x++) {
			for (int y = -routerRange; y <= routerRange; y++) {
				if (isCover(maxPotentiel.getCoordX(), maxPotentiel.getCoordY(), maxPotentiel.getCoordX() + x, maxPotentiel.getCoordY() + y)) {
					mapSortie[maxPotentiel.getCoordX() + x][maxPotentiel.getCoordY() + y].setType(COVERED);
				}
			}
		}

		//update de la map de solution couverture wifi après avoir update la couverture
		for (int x = -2 * routerRange; x <= 2 * routerRange; x++) {
			for (int y = -2 * routerRange; y <= 2 * routerRange; y++) {
				mapSearchCov[maxPotentiel.getCoordX() + x][maxPotentiel.getCoordY() + y] = potentielWifi(maxPotentiel.getCoordX() + x, maxPotentiel.getCoordY() + y);
			}
		}

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
		//FIN CC
	}
	std::cout << "Sorting des cables" << std::endl;
	Point backbone(backboneRow, backboneCol, CABLE);
	//suppression du point backbone avant de sort, il avait été utile pour trouver le plus court distance router cable
	//cables.erase(cables.begin());
	sorting(cables, cablesSorted, backbone);
}



//Fonction qui permet de ranger les cables de manière souhaitée par les règles du challenge
void sorting(const std::vector<Point> & listeRef, std::vector<Point> & liste, const Point & ptCentre) {
	for (auto &pt : listeRef) {
		if (pt.voisinDe(ptCentre) && find(liste.begin(), liste.end(), pt) == liste.end()) {
			liste.push_back(pt);
			sorting(listeRef, liste, pt);
		}
	}
}

//Fonction qui verifie si une cellule B est couverte si on place un routeur sur la cellule A 
bool ProblemData::isCover(const Point& ptA, const Point& ptB) const {
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

//Fonction similaire qui utilise des int
bool ProblemData::isCover(int ptAx, int ptAy, int ptBx, int ptBy) const {
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

//Fonction qui calcule la score obtenu à partir d'une liste de routeurs
long ProblemData::scoreRouters() {
	long score = 0;
	for (auto &router : routers) {
		for (int x = -routerRange; x <= routerRange; x++) {
			for (int y = -routerRange; y <= routerRange; y++) {
				if (mapSortie[router.getCoordX() + x][router.getCoordY() + y].getType() != COVERED && isCover(router, mapEntree[router.getCoordX() + x][router.getCoordY() + y])) {
					score += 1000;
					mapSortie[router.getCoordX() + x][router.getCoordY() + y].setType(COVERED);//Besoin de dire COVERED pour eviter de compter 2 fois la meme cellule
				}
			}
		}
	}
	return score;
}

//Fonction qui calcule le potentiel maximum de la carte si l'on arrive à la couvrir complètement
long ProblemData::calculMaxMoney() const {
	long somme = 0;
	for (auto &x : mapEntree) {
		for (auto &y : x) {
			if (y.getType() == TARGET)
			{
				somme += 1;
			}
		}
	}
	return somme * 1000;
}

//Redéfinition de l'opérateur pour afficher l'ensemble des informations du problème
std::ostream& operator<<(std::ostream& os, const ProblemData& data) {
	os << "Lignes : " << data.row << std::endl;
	os << "Colonnes : " << data.col << std::endl;
	os << "Portee routeur : " << data.routerRange << std::endl;
	os << "Prix de connection du cable : " << data.connectPrice << std::endl;
	os << "Prix routeur : " << data.routerPrice << std::endl;
	os << "Budget : " << data.maxBudget << std::endl;
	os << "Backbone situe en " << data.backboneRow << "," << data.backboneCol << std::endl;
	return os;
}

