#ifndef _country_h_
#define _country_h_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Competitor;

class Country {
	string name;
	vector<Competitor*> competitors;
public:
	Country(string name) : name(name) {}

	Country& addCompetitor(Competitor* c) { competitors.push_back(c); return *this; }
	string getName() const { return name; }
	vector<Competitor*>& getCompetitors() { return competitors; }

	bool operator<(const Country& c) const {
		return name < c.name;
	}
	bool operator==(const Country& c) {
		return name == c.name;
	}
	bool operator!=(const Country& c) {
		return !(*this == c);
	}
	friend ostream& operator<<(ostream& ot, const Country& c) {
		ot << c.name;
		return ot;
	}
};

#endif // !_country_h_
