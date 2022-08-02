#ifndef _athlete_h_
#define _athlete_h_

#include <iostream>
#include <string>
#include "Competitor.h"
using namespace std;

class Athlete : public Competitor {
	int id;
	string name;
	char sex;
	int age;
	int height;
	int weight;
public:
	Athlete(int id, string name, char sex, int age = -1, int height = -1, int weight = -1) :
		id(id), name(name), sex(sex), age(age), height(height), weight(weight) {}
	Athlete(Games* game, Event* eventt, Country* country, string medal, int id, string name, char sex, int age = -1, int height = -1, int weight = -1) :
		Competitor(game, eventt, country, medal), id(id), name(name), sex(sex), age(age), height(height), weight(weight) {}
	
	int getID() const { return id; }
	string getName() const { return name; }
	char getSex() const { return sex; }
	int getAge() const { return age; }
	int getHeight() const { return height; }
	int getWeight() const { return weight; }

	bool operator<(const Athlete& a) const {
		return id < a.id;
	}
	bool operator==(const Athlete& a) {
		return id == a.id && name == a.name && sex == a.sex
			&& age == a.age && height == a.height && weight == a.weight;
	}
	bool operator!=(const Athlete& a) {
		return !(*this == a);
	}
	friend ostream& operator<<(ostream& ot, const Athlete& a) {
		ot << a.id << " " << a.name << " " << a.sex;
		if (a.age != -1) ot << " " << a.age;
		if (a.height != -1) ot << " " << a.height;
		if (a.weight != -1) ot << " " << a.weight;
		return ot;
	}
};

#endif // !_athlete_h_
