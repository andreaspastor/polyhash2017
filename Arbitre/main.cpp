#include <iostream>
#include "Parser.h"

using namespace std;

int main()
{
	cout << "Projet ARBITRE" << endl;

	Parser parser;
	parser.ParseRule("opera.in");
	cout << "Parse rule OK" << endl;
	parser.ParseAnswer("coord.txt");
	cout << "Parse answer OK" << endl;

	cout << "Budget respected ? " << parser.isBudgetRespected() << endl;

	system("Pause");

	return 0;
}