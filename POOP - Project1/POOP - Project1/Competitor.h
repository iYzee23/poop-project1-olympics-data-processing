#ifndef _competitor_h_
#define _competitor_h_

#include <iostream>
#include <string>
#include "Games.h"
#include "Event.h"
#include "Country.h"
using namespace std;

class Competitor {
protected:
	Games* game = nullptr;
	Event* eventt = nullptr;
	Country* country = nullptr;
	string medal = "";
	bool team = false;
public:
	Competitor() = default;
	Competitor(Games* game, Event* eventt, Country* country, string medal) :
		game(game), eventt(eventt), country(country), medal(medal) { 
		game->addCompetitor(this);
		country->addCompetitor(this);
	}

	Games* getGame() { return game; }
	void setGame(Games* g) { game = g; game->addCompetitor(this); }
	Event* getEvent() { return eventt; }
	void setEvent(Event* e) { eventt = e; }
	Country* getCountry() { return country; }
	void setCountry(Country* c) { country = c; country->addCompetitor(this); }
	string getMedal() const { return medal; }
	void setMedal(string m) { medal = m; }
	bool getTeam() const { return team; }
};

#endif // !_competitor_h_
