#ifndef _team_h_
#define _team_h_

#include <iostream>
#include <vector>
#include <algorithm>
#include "Competitor.h"
using namespace std;

class Athlete;

class Team : public Competitor {
	vector<Athlete*> athletes;
public:
	Team() : Competitor() { team = true; }
	Team(Games* game, Event* eventt, Country* country, string medal) :
		Competitor(game, eventt, country, medal) { team = true; }

	Team& addAthlete(Athlete* ath) { athletes.push_back(ath); return *this; }
	vector<Athlete*>& getAthletes() { return athletes; }

	friend ostream& operator<<(ostream& ot, const Team& t) {
		bool first = true;
		ot << "[";
		for_each(t.athletes.begin(), t.athletes.end(), [&](Athlete* a) {
			if (first) {
				ot << a->getID();
				first = false;
			}
			else ot << ", " << a->getID();
		});
		return ot << "]";
	}
};

#endif // !_team_h_
