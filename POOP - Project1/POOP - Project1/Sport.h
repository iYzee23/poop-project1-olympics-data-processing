#ifndef _sport_h_
#define _sport_h_

#include <iostream>
#include <string>
#include <set>
using namespace std;

class Event;

class Sport {
	string name;
	set<Event> events;
public:
	Sport(string name) : name(name) {}

	Sport& addEvent(Event* e) { events.insert(*e); return *this; }
	string getName() const { return name; }
	set<Event>& getEvents() { return events; }

	bool operator<(const Sport& s) const {
		return name < s.name;
	}
	bool operator==(const Sport& s) {
		return name == s.name;
	}
	bool operator!=(const Sport& s) {
		return !(*this == s);
	}
	friend ostream& operator<<(ostream& ot, const Sport& s) {
		ot << s.name;
		return ot;
	}
};

#endif // !_sport_h_
