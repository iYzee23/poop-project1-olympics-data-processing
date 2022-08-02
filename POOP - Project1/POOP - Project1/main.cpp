#include "OlimpicGames.h"

OlimpicGames* og = nullptr;
int option, year, yearTmp, resInt;
string sport, country, type, typeTmp, medal;
double resDouble;
vector<Country*> resVectorCountry;
list<pair<Country*, Athlete*>> resListPairCountryAthlete;
vector<Athlete*> resVectorAthlete;
list<Team*> resListTeam;
set<string> resSetString;

void printMenu() {
	cout << "Menu:" << endl;
	cout << "0 - Finish the program" << endl;
	cout << "1 - Get number of competitors on Olimpic games" << endl;
	cout << "2 - Get number of events on Olimpic games" << endl;
	cout << "3 - Get average height of athletes" << endl;
	cout << "4 - Get average weight of athletes" << endl;
	cout << "5 - Get number of sports in which given country has won at least one medal" << endl;
	cout << "6 - Get three best countries by success on given Olimpic games" << endl;
	cout << "7 - Get countries that were best by success on at least one Olimpic games" << endl;
	cout << "8 - Get ten youngest competitors on Olimpic games that have won medal on the first appearance" << endl;
	cout << "// 9 - Get pairs of \"country --> athlete\" for all the athletes that have won both individual and team medal" << endl;
	cout << "// 10 - Get athletes that have participated on given pair of Olimpic games" << endl;
	cout << "// 11 - Get teams that given country had on given Olimpic games" << endl;
	cout << "12 - Get all the cities in which Olimpic games were held at least once" << endl;
	cout << "Your option: ";
}
void applyFilter() {
	getchar();
	sport = country = type = medal = "";
	year = -1;
	cout << "Filter your option. Enter \"-1\" if doesn't matter" << endl;
	cout << "Sport? "; getline(cin, sport); if (sport == "-1") sport = "";
	cout << "Country? "; getline(cin, country); if (country == "-1") country = "";
	cout << "Year? "; cin >> year; getchar();
	cout << "Type? "; getline(cin, type); if (type == "-1") type = "";
	cout << "Medal? "; getline(cin, medal); if (medal == "-1") medal = "";
}
void fMode() {
	y:
	cout << "Choose mode (1 - group, 2 - individual): ";
	cin >> option;
	switch (option) {
		case 1:
			og->parse();
			cout << endl;
			break;
		case 2:
			x:
			cout << "Choose year: ";
			cin >> option;
			if (option < 0) {
				cout << "You entered wrong year. Try again!" << endl;
				goto x;
			}
			og->parse(option);
			cout << endl;
			break;
		default:
			cout << "Wrong option. Try again!" << endl << endl;
			goto y;
	}
}
void fMenu() {
	while (true) {
		printMenu();
		z:
		cin >> option;
		switch (option) {
			case 0:
				return;
			case 1:
				applyFilter();
				resInt = og->getNumOfCompetitors(sport, country, year, type, medal);
				cout << "Number of competitors: " << resInt << endl << endl;
				break;
			case 2:
				applyFilter();
				resInt = og->getNumOfEvents(sport, country, year, type, medal);
				cout << "Number of events: " << resInt << endl << endl;
				break;
			case 3:
				applyFilter();
				resDouble = og->getAvgHeight(sport, country, year, type, medal);
				cout << "Average height: " << resDouble << endl << endl;
				break;
			case 4:
				applyFilter();
				resDouble = og->getAvgWeight(sport, country, year, type, medal);
				cout << "Average weight: " << resDouble << endl << endl;
				break;
			case 5:
				cout << "Country? "; getchar(); getline(cin, country);
				resInt = og->getDifferentSports(country);
				cout << "Number of sports: " << resInt << endl << endl;
				break;
			case 6:
				cout << "Year? "; cin >> year;
				cout << "Summer/Winter? "; cin >> type;
				resVectorCountry = og->getThreeBestCountries(year, type);
				cout << "Countries:\n";
				for_each(resVectorCountry.begin(), resVectorCountry.end(), [](Country* c) { cout << *c << endl; });
				cout << endl;
				break;
			case 7:
				resVectorCountry = og->getGeneralBestCountries();
				cout << "Countries:\n";
				for_each(resVectorCountry.begin(), resVectorCountry.end(), [](Country* c) { cout << *c << endl; });
				cout << endl;
				break;
			case 8:
				resVectorAthlete = og->getTenYoungestMedalistOnFirstOlimpics();
				cout << "Competitors:\n";
				for_each(resVectorAthlete.begin(), resVectorAthlete.end(), [](Athlete* a) { cout << *a << endl; });
				cout << endl;
				break;
			case 9:
				resListPairCountryAthlete = og->getPairsCountryAthlete();
				cout << "Pairs \"country --> athlete\":\n";
				for_each(resListPairCountryAthlete.begin(), resListPairCountryAthlete.end(), [](pair<Country*, Athlete*> p) {
					cout << *p.first << " --> " << *p.second << endl;
				});
				cout << endl;
				break;
			case 10:
				cout << "First games' year? "; cin >> year;
				cout << "First games' Summer/Winter? "; cin >> type;
				cout << "Second games' year? "; cin >> yearTmp;
				cout << "Second games' Summer/Winter? "; cin >> typeTmp;
				resVectorAthlete = og->getAthletesOnGivenOlimpicsPair({ {year, type}, {yearTmp, typeTmp} });
				cout << "Athletes:\n";
				for_each(resVectorAthlete.begin(), resVectorAthlete.end(), [](Athlete* a) { cout << *a << endl; });
				cout << endl;
				break;
			case 11:
				cout << "Country? "; getchar(); getline(cin, country);
				cout << "Year? "; cin >> year;
				cout << "Summer/Winter? "; cin >> type;
				resListTeam = og->getSortedTeams(country, year, type);
				cout << "Teams:\n";
				for_each(resListTeam.begin(), resListTeam.end(), [](Team* t) { cout << *t << endl; });
				cout << endl;
				break;
			case 12:
				resSetString = og->getCities();
				cout << "Cities:\n";
				for_each(resSetString.begin(), resSetString.end(), [](string s) { cout << s << endl; });
				cout << endl;
				break;
			default:
				cout << "Wrong option. Try again!" << endl;
				goto z;
		}
	}
}

int main() {
	og = OlimpicGames::getInstance();
	
	fMode();
	fMenu();
	
	OlimpicGames::deleteInstance();
}