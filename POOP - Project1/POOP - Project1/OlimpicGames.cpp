#include "OlimpicGames.h"
#include <fstream>
#include <regex>

OlimpicGames* OlimpicGames::instance = nullptr;

OlimpicGames* OlimpicGames::getInstance() {
	if (!instance) instance = new OlimpicGames();
	return instance;
}

void OlimpicGames::deleteInstance() {
	if (instance) delete instance;
}

void OlimpicGames::parse(int y) {
	ifstream ifs1(".\\athletes.txt");
	string s1;
	regex rx1("([^!]*)!([^!]*)!([^!]*)!([^!]*)!([^!]*)!([^\\n]*)");
	while (getline(ifs1, s1)) {
		smatch result1;
		if (regex_match(s1, result1, rx1)) {
			int id = atoi(result1.str(1).c_str());
			string name = result1.str(2);
			char sex = result1.str(3).c_str()[0];
			string age = result1.str(4);
			string height = result1.str(5);
			string weight = result1.str(6);
			// has to be dynamic allocation, cannot be Athlete a(...);
			// otherwise, undefined data after we exit activate block
			Athlete* a = new Athlete(id, name, sex, 
				(age != "NA" ? atoi(age.c_str()) : -1), 
				(height != "NA" ? atoi(height.c_str()) : -1), 
				(weight != "NA" ? atoi(weight.c_str()) : -1)
			);
			allAthletes[id] = a;
		}
	}
	ifs1.close();

	ifstream ifs2(".\\events.txt");
	string s2;
	regex rx2("([^ ]*) ([^!]*)!([^!]*)!([^!]*)!([^!]*)!([^!]*)!([^!]*)!([^!]*)!([^\\n]*)");
	regex rx3("'([^']+)'");
	while (getline(ifs2, s2)) {
		smatch result2;
		if (regex_match(s2, result2, rx2)) {
			int year = atoi(result2.str(1).c_str());
			if (y != -1 && y != year) continue;
			string typeSW = result2.str(2);
			string city = result2.str(3);
			string sport = result2.str(4);
			string eventt = result2.str(5);
			string type = result2.str(6);
			string country = result2.str(7);
			string ids = result2.str(8);
			string medal = (result2.str(9) != "" ? result2.str(9) : "None");
			Games* g = new Games(year, typeSW, city); 
			allGames.insert(pair<pair<int, string>, Games*>({year, typeSW}, g));
			g = allGames[pair<int, string>(year, typeSW)];
			Country* c = new Country(country); 
			allCountries.insert(pair<string, Country*>(country, c));
			c = allCountries[country];
			Sport* s = new Sport(sport); 
			allSports.insert(pair<string, Sport*>(sport, s));
			s = allSports[sport];
			Event* e = new Event(eventt, type, s); 
			allEvents.insert(pair<string, Event*>(eventt, e));
			e = allEvents[eventt];
			
			if (type == "Individual") {
				Athlete* ath = allAthletes[atoi(ids.c_str())];
				Athlete* cmpt = new Athlete(g, e, c, medal, ath->getID(), ath->getName(), ath->getSex(), ath->getAge(), ath->getHeight(), ath->getWeight());
				ath->setCountry(c);
				allCompetitors.push_back(cmpt);
			}
			else {
				Team* cmpt = new Team(g, e, c, medal);
				sregex_iterator begin(ids.begin(), ids.end(), rx3);
				sregex_iterator end;
				while (begin != end) {
					smatch sm = *begin;
					Athlete* ath = allAthletes[atoi(sm.str(1).c_str())];
					ath->setCountry(c);
					cmpt->addAthlete(ath);
					begin++;
				}
				allCompetitors.push_back(cmpt);
			}
		}
	}
	ifs2.close();
}

int OlimpicGames::getNumOfCompetitors(string sport, string country, int year, string type, string medal) {
	set<int> result;
	vector<Competitor*> cmpts = filter(allCompetitors, sport, country, year, type, medal);
	for (auto it : cmpts) {
		if (!it->getTeam()) result.insert(((Athlete*)it)->getID());
		else {
			vector<Athlete*> ath = ((Team*)it)->getAthletes();
			for_each(ath.begin(), ath.end(), [&result](Athlete* a) { result.insert(a->getID()); });
		}
	}
	return result.size();
}

int OlimpicGames::getNumOfEvents(string sport, string country, int year, string type, string medal) {
	// for every set, template class should have overwritten operator<
	// operator< is used to compare objects put in set
	// if we override it as a global function: friend bool operator<(...) { ... }
	// if we override it as a member function: bool operator<(...) const { ... }
	set<Event> result;
	vector<Competitor*> cmpts = filter(allCompetitors, sport, country, year, type, medal);
	for (auto it : cmpts) result.insert(*it->getEvent());
	return result.size();
}

double OlimpicGames::getAvgHeight(string sport, string country, int year, string type, string medal) {
	set<int> result;
	vector<Competitor*> cmpts = filter(allCompetitors, sport, country, year, type, medal);
	for (auto it : cmpts) {
		if (!it->getTeam() && ((Athlete*)it)->getHeight() != -1) result.insert(((Athlete*)it)->getID());
		else if (it->getTeam()){
			vector<Athlete*> ath = ((Team*)it)->getAthletes();
			for_each(ath.begin(), ath.end(), [&result](Athlete* a) { 
				if (a->getHeight() != -1) result.insert(a->getID()); 
			});
		}
	}
	if (!result.size()) return -1;
	double res = 0;
	for_each(result.begin(), result.end(), [&](int a) { res += allAthletes[a]->getHeight(); });
	return res / result.size();
}

double OlimpicGames::getAvgWeight(string sport, string country, int year, string type, string medal) {
	set<int> result;
	vector<Competitor*> cmpts = filter(allCompetitors, sport, country, year, type, medal);
	for (auto it : cmpts) {
		if (!it->getTeam() && ((Athlete*)it)->getWeight() != -1) result.insert(((Athlete*)it)->getID());
		else if (it->getTeam()) {
			vector<Athlete*> ath = ((Team*)it)->getAthletes();
			for_each(ath.begin(), ath.end(), [&result](Athlete* a) {
				if (a->getWeight() != -1) result.insert(a->getID());
				});
		}
	}
	double res = 0;
	for_each(result.begin(), result.end(), [&](int a) { res += allAthletes[a]->getWeight(); });
	return res / result.size();
}

int OlimpicGames::getDifferentSports(string country) {
	Country* ctr = allCountries[country];
	if (!ctr) return -1;
	set<Sport> result;
	for_each(ctr->getCompetitors().begin(), ctr->getCompetitors().end(), [&](auto c) { 
		if (c->getMedal() != "None") result.insert(*c->getEvent()->getSport()); 
	});
	return result.size();
}

vector<Country*> OlimpicGames::getThreeBestCountries(int year, string typeSW) {
	Games* game = allGames[{year, typeSW}];
	if (!game) return {};
	map<string, tuple<int, int, int >> mapa;
	for_each(game->getCompetitors().begin(), game->getCompetitors().end(), [&](Competitor* c) {
		if (c->getMedal() == "Gold") get<0>(mapa[c->getCountry()->getName()])++;
		else if (c->getMedal() == "Silver") get<1>(mapa[c->getCountry()->getName()])++;
		else if (c->getMedal() == "Bronze") get<2>(mapa[c->getCountry()->getName()])++;
	});
	// thus map implementation sorts the map in some order, we cannot sort it by values
	// we first have to put all the [key, value] in vector, and then sort the vector
	using PT = pair<string, tuple<int, int, int >>;
	vector<PT> helper(mapa.begin(), mapa.end());
	sort(helper.begin(), helper.end(), [&](PT p1, PT p2) {
		if (get<0>(p1.second) > get<0>(p2.second)) return true;
		else if (get<0>(p1.second) < get<0>(p2.second)) return false;
		else {
			if (get<1>(p1.second) > get<1>(p2.second)) return true;
			else if (get<1>(p1.second) < get<1>(p2.second)) return false;
			else {
				if (get<2>(p1.second) < get<2>(p2.second)) return false;
				else return true;
			}
		}
	});
	vector<Country*> result;
	// since there is no for_each_n, we imitate it this way
	for_each(helper.begin(), (helper.size() > 3 ? next(helper.begin(), 3) : helper.end()), [this, &result](PT p) {
		result.push_back(allCountries[p.first]);
	});
	return result;
}

vector<Country*> OlimpicGames::getGeneralBestCountries() {
	if (!allGames.size()) return {};
	using PT = pair<string, tuple<int, int, int>>;
	set<string> resHelp;
	for_each(allGames.begin(), allGames.end(), [&resHelp](pair<pair<int, string>, Games*> p) {
		Games* game = p.second;
		map<string, tuple<int, int, int>> mapa;
		for_each(game->getCompetitors().begin(), game->getCompetitors().end(), [&](Competitor* c) {
			if (c->getMedal() == "Gold") get<0>(mapa[c->getCountry()->getName()])++;
			else if (c->getMedal() == "Silver") get<1>(mapa[c->getCountry()->getName()])++;
			else if (c->getMedal() == "Bronze") get<2>(mapa[c->getCountry()->getName()])++;
		});
		vector<PT> helper(mapa.begin(), mapa.end());
		// since we are sorting the array in desceding order, we look for minimum element in sort result
		PT res = *min_element(helper.begin(), helper.end(), [&](PT p1, PT p2) {
			if (get<0>(p1.second) > get<0>(p2.second)) return true;
			else if (get<0>(p1.second) < get<0>(p2.second)) return false;
			else {
				if (get<1>(p1.second) > get<1>(p2.second)) return true;
				else if (get<1>(p1.second) < get<1>(p2.second)) return false;
				else {
					if (get<2>(p1.second) < get<2>(p2.second)) return false;
					else return true;
				}
			}
		});
		resHelp.insert(res.first);
	});
	
	vector<Country*> result;
	for_each(resHelp.begin(), resHelp.end(), [this, &result](string c) {
		result.push_back(allCountries[c]);
	});
	return result;
}

vector<Athlete*> OlimpicGames::getTenYoungestMedalistOnFirstOlimpics() {
	vector<Games*> games;
	for (auto it : allGames) games.push_back(it.second);
	sort(games.begin(), games.end(), [](Games* g1, Games* g2) { return g1->getYear() < g2->getYear(); });
	set<int> helpAth;
	vector<Athlete*> result;
	for_each(games.begin(), games.end(), [&helpAth, &result](Games* g) {
		for_each(g->getCompetitors().begin(), g->getCompetitors().end(), [&](Competitor* c) {
			if (!c->getTeam() && helpAth.find(((Athlete*)c)->getID()) == helpAth.end() && c->getMedal() != "None") {
				result.push_back((Athlete*)c);
				helpAth.insert(((Athlete*)c)->getID());
			}
		});
	});
	sort(result.begin(), result.end(), [](Athlete* a1, Athlete* a2) { return a1->getAge() < a2->getAge(); });
	if (result.size() > 10) result.erase(next(result.begin(), 10), result.end());
	return result;
}

list<pair<Country*, Athlete*>> OlimpicGames::getPairsCountryAthlete() {
	// first boolean in pair is true if Athlete has won individual medal
	// second boolean in pair is true if Athlete has won team medal
	map<int, pair<bool, bool>> mapa;
	for_each(allCompetitors.begin(), allCompetitors.end(), [&mapa](Competitor* c) {
		if (!c->getTeam() && c->getMedal() != "None") get<0>(mapa[((Athlete*)c)->getID()]) = true;
		else if (c->getTeam() && c->getMedal() != "None") {
			for (auto it : ((Team*)c)->getAthletes()) get<1>(mapa[it->getID()]) = true;
		}
	});
	using PT = pair<Country*, Athlete*>;
	list<PT> result;
	for_each(mapa.begin(), mapa.end(), [this, &result](pair<int, pair<bool, bool>> p) {
		if (get<0>(p.second) && get<1>(p.second)) 
			result.push_back(PT{allAthletes[p.first]->getCountry(), allAthletes[p.first]});
	});
	return result;
}

vector<Athlete*> OlimpicGames::getAthletesOnGivenOlimpicsPair(pair<pair<int, string>, pair<int, string>> twoGames) {
	Games* game1 = allGames[twoGames.first];
	Games* game2 = allGames[twoGames.second];
	set<int> s1, s2;
	for_each(game1->getCompetitors().begin(), game1->getCompetitors().end(), [&s1](Competitor* c) {
		if (!c->getTeam()) s1.insert(((Athlete*)c)->getID());
		else for (auto it : ((Team*)c)->getAthletes()) s1.insert(it->getID());
	});
	for_each(game2->getCompetitors().begin(), game2->getCompetitors().end(), [&s2](Competitor* c) {
		if (!c->getTeam()) s2.insert(((Athlete*)c)->getID());
		else for (auto it : ((Team*)c)->getAthletes()) s2.insert(it->getID());
	});
	vector<int> resHelp;
	set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(resHelp));
	vector<Athlete*> result;
	for_each(resHelp.begin(), resHelp.end(), [this, &result](int a) { result.push_back(allAthletes[a]); });
	return result;
}

list<Team*> OlimpicGames::getSortedTeams(string country, int year, string typeSW) {
	Country* ctr = allCountries[country];
	if (!ctr) return {};
	Games* game = allGames[{year, typeSW}];
	if (!game) return {};
	using PT = pair<Team*, pair<int, string>>;
	vector<PT> helper;
	for_each(game->getCompetitors().begin(), game->getCompetitors().end(), [ctr, &helper](Competitor* c) {
		if (c->getTeam() && *c->getCountry() == *ctr) {
			int num = ((Team*)c)->getAthletes().size();
			string name = c->getEvent()->getName();
			helper.push_back({ (Team*)c, {num, name} });
		}
	});
	list<Team*> result;
	sort(helper.begin(), helper.end(), [](PT p1, PT p2) {
		if (p1.second.first > p2.second.first) return true;
		else if (p1.second.first < p2.second.first) return false;
		else if (p1.second.second <= p2.second.second) return true;
		else return false;
	});
	for_each(helper.begin(), helper.end(), [&result](PT p) { result.push_back(p.first); });
	return result;
}

set<string> OlimpicGames::getCities() {
	set<string> result;
	for_each(allGames.begin(), allGames.end(), [&result](pair<pair<int, string>, Games*> p) {
		result.insert(p.second->getCity());
	});
	return result;
}

vector<Competitor*> OlimpicGames::filter(vector<Competitor*> base, string sport, string country, int year, string type, string medal) {
	vector<Competitor*> result = base;

	if (sport != "") {
		Sport* spr = allSports[sport];
		if (!spr) return {};
		auto it = remove_if(result.begin(), result.end(), [spr](Competitor* c) { return *c->getEvent()->getSport() != *spr; });
		result.erase(it, result.end());

	}
	if (country != "") {
		Country* ctr = allCountries[country];
		if (!ctr) return {};
		auto it = remove_if(result.begin(), result.end(), [ctr](Competitor* c) { return *c->getCountry() != *ctr; });
		result.erase(it, result.end());
	}
	if (year != -1) {
		vector<Games*> gms;
		// if we had pointer (ie Games* gms), we would had have to use reference on pointer
		// that means we would also write &gms
		for_each(allGames.begin(), allGames.end(), [&gms, year](pair<pair<int, string>, Games*> g) { 
			if (g.second->getYear() == year) gms.push_back(g.second);
		});
		if (!gms.size()) return {};
		bool ind = true;
		auto it = remove_if(result.begin(), result.end(), [gms, &ind](Competitor* c) { 
			for_each(gms.begin(), gms.end(), [c, &ind](Games* g) { if (*c->getGame() == *g) { ind = false; return; } });
			return ind;
		});
		result.erase(it, result.end());
	}
	if (type != "") {
		vector<Event*> evs;
		for_each(allEvents.begin(), allEvents.end(), [&evs, type](pair<string, Event*> e) { 
			if (e.second->getType() == type) evs.push_back(e.second);
		});
		if (!evs.size()) return {};
		bool ind = true;
		auto it = remove_if(result.begin(), result.end(), [evs, &ind](Competitor* c) {
			for_each(evs.begin(), evs.end(), [c, &ind](Event* e) { if (*c->getEvent() == *e) { ind = false; return; } });
			return ind;
		});
		result.erase(it, result.end());
	}
	if (medal != "") {
		auto it = remove_if(result.begin(), result.end(), [medal](Competitor* c) { return c->getMedal() != medal; });
		result.erase(it, result.end());
	}

	return result;
}

ostream& operator<<(ostream& ot, const OlimpicGames& og) {
	ot << "Games:" << endl;
	for_each(og.allGames.begin(), og.allGames.end(), [&](pair<pair<int, string>, Games*> p) { ot << "\t" << *p.second << endl; });
	ot << "Countries:" << endl;
	for_each(og.allCountries.begin(), og.allCountries.end(), [&](pair<string, Country*> p) { ot << "\t" << *p.second << endl; });
	ot << "Events:" << endl;
	for_each(og.allEvents.begin(), og.allEvents.end(), [&](pair<string, Event*> p) { ot << "\t" << *p.second << endl; });
	ot << "Athletes:" << endl;
	for_each(og.allAthletes.begin(), next(og.allAthletes.begin(), 10), [&](pair<int, Athlete*> p) { ot << "\t" << *p.second << endl; });
	
	return ot;
}
