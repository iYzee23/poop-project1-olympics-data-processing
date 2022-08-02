#ifndef _olimpic_games_h_
#define _olimpic_games_h_

#include "Athlete.h"
#include "Competitor.h"
#include "Country.h"
#include "Event.h"
#include "Games.h"
#include "Sport.h"
#include "Team.h"
#include <map>
#include <set>
#include <tuple>
#include <algorithm>
#include <list>
using namespace std;

class OlimpicGames {
public:
	static OlimpicGames* instance;
	map<pair<int, string>, Games*> allGames;
	map<string, Country*> allCountries;
	map<string, Sport*> allSports;
	map<string, Event*> allEvents;
	map<int, Athlete*> allAthletes;
	vector<Competitor*> allCompetitors;
	OlimpicGames() {}
public:
	static OlimpicGames* getInstance();
	static void deleteInstance();
	void parse(int y = -1);

	int getNumOfCompetitors(string sport = "", string country = "", int year = -1, string type = "", string medal = "");
	int getNumOfEvents(string sport = "", string country = "", int year = -1, string type = "", string medal = "");
	double getAvgHeight(string sport = "", string country = "", int year = -1, string type = "", string medal = "");
	double getAvgWeight(string sport = "", string country = "", int year = -1, string type = "", string medal = "");

	int getDifferentSports(string country);
	vector<Country*> getThreeBestCountries(int year, string typeSW);
	vector<Country*> getGeneralBestCountries();
	vector<Athlete*> getTenYoungestMedalistOnFirstOlimpics();
	list<pair<Country*, Athlete*>> getPairsCountryAthlete();
	vector<Athlete*> getAthletesOnGivenOlimpicsPair(pair<pair<int, string>, pair<int, string>> twoGames);
	list<Team*> getSortedTeams(string country, int year, string typeSW);
	set<string> getCities();

	friend ostream& operator<<(ostream& ot, const OlimpicGames& og);
	vector<Competitor*> filter(vector<Competitor*>, string sport, string country, int year, string type, string medal);
};

#endif // !_olimpic_games_h_
