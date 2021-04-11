#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <ctime>
#include "Item.h"
#include "Date.h"

using namespace std;

Item::Item() {

	random_device r;
	default_random_engine generator(r());

	uniform_int_distribution<int> rGroup(0, 25);
	Group = 'A' + rGroup(generator);

	uniform_int_distribution<int> rSubgroup(0, 99);
	Subgroup = rSubgroup(generator);

	vector<string> lines;
	string line;
	ifstream file("Birds.txt");
	while (getline(file, line)) {
		if(!line.empty())
			lines.push_back(line);
	}
	uniform_int_distribution<int> rName(0, (int)lines.size() - 1);
	Name = lines[rName(generator)];
	file.close();

	Timestamp = Date::CreateRandomDate(Date(6, 6, 2001), Date(6, 6, 2021));
}

Item::Item(char group, int subgroup, string name, Date timestamp){
	Group = group;
	Subgroup = subgroup;
	Name = name;
	Timestamp = timestamp;
}

Item::Item(const Item& original) {
	Group = original.Group;
	Subgroup = original.Subgroup;
	Name = original.Name;
	Timestamp = original.Timestamp;
}

Item::~Item() {}

char Item::getGroup() {
	return Group;
}

int Item::getSubgroup(){
	return Subgroup;
}

string Item::getName() {
	return Name;
}

Date Item::getTimestamp() {
	return Timestamp;
}