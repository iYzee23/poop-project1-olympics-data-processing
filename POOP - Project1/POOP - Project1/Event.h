#ifndef _event_h_
#define _event_h_

#include <iostream>
#include <string>
#include "Sport.h"
using namespace std;

class Event {
	string name;
	string type;
	Sport* sport;
public:
	Event(string name, string type, Sport* sport) :
		name(name), type(type), sport(sport) { sport->addEvent(this); }

	string getName() const { return name; }
	string getType() const { return type; }
	Sport* getSport() { return sport; }
	bool operator<(const Event& e) const {
		return name < e.name;
	}
	bool operator==(const Event& e) {
		return name == e.name && type == e.type && *sport == *e.sport;
	}
	bool operator!=(const Event& e) {
		return !(*this == e);
	}
	friend ostream& operator<<(ostream& ot, const Event& e) {
		ot << *e.sport << " " << e.name << " " << e.type;
		return ot;
	}
};

#endif // !_event_h_
