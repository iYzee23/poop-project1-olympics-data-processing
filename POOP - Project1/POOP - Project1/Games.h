#ifndef _games_h_
#define _games_h_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Competitor;

class Games {
	int year;
	string type;
	string city;
	vector<Competitor*> competitors;
public:
	Games(int year, string type, string city) :
		year(year), type(type), city(city) {}

	Games& addCompetitor(Competitor* c) { competitors.push_back(c); return* this; }
	int getYear() const { return year; }
	string getType() const { return type; }
	string getCity() const { return city; }
	vector<Competitor*>& getCompetitors() { return competitors; }
	
	bool operator==(const Games& g) {
		return year == g.year && type == g.type && city == g.city;
	}
	bool operator!=(const Games& g) {
		return !(*this == g);
	}
	friend ostream& operator<<(ostream& ot, const Games& g) {
		ot << g.year << " " << g.type << " " << g.city;
		return ot;
	}
};

#endif // !_games_h_
