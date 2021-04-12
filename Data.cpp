#pragma once
#include <iostream>
#include <string>
#include "Data.h"

Data::Data(int n) {
	while (n--) {
		Item* temp = new Item();
		char g = temp->getGroup();
		char sg = temp->getSubgroup();

		if (DataStructure.count(g) > 0) {
			if ((*DataStructure[g]).count(sg) > 0) {
				(*DataStructure[g])[sg]->push_back(temp);
			}
			else {
				auto l = new list<Item*>{ temp };
				(*DataStructure[g])[sg] = l;
			}
		}
		else {
			auto l = new list<Item*>{ temp };
			auto m = new map<int, list<Item*>*>{ {sg, l} };
			DataStructure[g] = m;
		}
	}
}

Data::Data() {
	DataStructure = map<char, map<int, list<Item*>*>*>{};
}

Data::~Data() {
	if (DataStructure.empty()) return;
	for (auto g : DataStructure) {
		for (auto sg : *g.second) {
			for (auto item : *sg.second)
				delete item;
			delete sg.second;
		}
		delete g.second;
	}
}

void Data::PrintAll() {
	for (const auto& g : DataStructure) {
		cout << g.first << ":\n";
		for (const auto& sg : *g.second) {
			if (sg.second->size() > 1) {
				sg.second->sort([](Item* i1, Item* i2) {
					for (int n = 0;; n++) {
						if (i1->getName()[n] < i2->getName()[n]) return true;
						else if (i1->getName()[n] == i2->getName()[n]) continue;
						else return false;
					}
					});
			}
			for (const auto& l : *sg.second)
				cout << sg.first << ": " << l->getName() << " " << l->getTimestamp().ToString() << endl;
		}
		cout << endl;
	}
}

int Data::CountItems() {
	int n = 0;
	for (const auto& g : DataStructure)
		for (const auto& sg : *g.second)
			for (const auto& l : *sg.second)
				n++;
	return n;
}

map<int, list<Item*>*>* Data::GetGroup(char c) {
	if (DataStructure.count(c) == 0) return nullptr;
	return DataStructure[c];
}

void Data::PrintGroup(char c) {
	try {
		if (DataStructure.count(c) == 0) throw invalid_argument("Group does not exist");
		for (const auto& sg : *DataStructure[c]) {
			if (sg.second->size() > 1) {
				sg.second->sort([](Item* i1, Item* i2) {
					for (int n = 0;; n++) {
						if (i1->getName()[n] < i2->getName()[n]) return true;
						else if (i1->getName()[n] == i2->getName()[n]) continue;
						else return false;
					}
					});
			}
			for (const auto& l : *sg.second)
				cout << sg.first << ": " << l->getName() << " " << l->getTimestamp().ToString() << endl;
		}
		cout << endl;
	}
	catch (const invalid_argument& e) {
		cout << e.what() << endl;
	}
}

int Data::CountGroupItems(char c) {
	if (DataStructure.count(c) == 0) return 0;
	int n = 0;
	for (const auto& sg : *DataStructure[c])
		for (const auto& l : *sg.second)
			n++;
	return n;
}

list<Item*>* Data::GetSubgroup(char c, int i) {
	if (DataStructure.count(c) == 0 || (*DataStructure[c]).count(i) == 0) return nullptr;
	return (*DataStructure[c])[i];
}

void Data::PrintSubgroupByNames(char c, int i) {
	try {
		if (DataStructure.count(c) == 0) throw invalid_argument("Group does not exist");
		if ((*DataStructure[c]).count(i) == 0) throw invalid_argument("Subgroup does not exist");
		if((*DataStructure[c])[i]->size() > 1){
			(*DataStructure[c])[i]->sort([](Item* i1, Item* i2) {
				for (int n = 0;; n++) {
					if (i1->getName()[n] < i2->getName()[n]) return true;
					else if (i1->getName()[n] == i2->getName()[n]) continue;
					else return false;
				}
				});
		}
		for (const auto& l : *(*DataStructure[c])[i])
			cout << l->getName() << " " << l->getTimestamp().ToString() << endl;
	}
	catch (const invalid_argument& e) {
		cout << e.what() << endl;
	}
}

void Data::PrintSubgroupByDates(char c, int i) {
	try {
		if (DataStructure.count(c) == 0) throw invalid_argument("Group does not exist");
		if ((*DataStructure[c]).count(i) == 0) throw invalid_argument("Subgroup does not exist");
		if ((*DataStructure[c])[i]->size() > 1) {
			(*DataStructure[c])[i]->sort([](Item* i1, Item* i2) {
				return i1->getTimestamp().operator<(i2->getTimestamp());
				});
		}
		for (const auto& l : *(*DataStructure[c])[i])
			cout << l->getName() << " " << l->getTimestamp().ToString() << endl;
	}
	catch (const invalid_argument& e) {
		cout << e.what() << endl;
	}
}

int Data::CountSubgroupItems(char c, int i) {
	if (DataStructure.count(c) == 0 || (*DataStructure[c]).count(i) == 0) return 0;
	int n = 0;
	for (const auto& l : *(*DataStructure[c])[i])
		n++;
	return n;
}

Item* Data::GetItem(char c, int i, string s){
	if (DataStructure.count(c) == 0 || (*DataStructure[c]).count(i) == 0) return nullptr;
	for (Item* item : *(*DataStructure[c])[i]) {
		if (!item->getName().compare(s)) return item;
	}
	return nullptr;
}

void Data::PrintItem(char c, int i, string s) {
	try {
		if (DataStructure.count(c) == 0 || (*DataStructure[c]).count(i) == 0) throw invalid_argument("Item does not exist");
		for (Item* item : *(*DataStructure[c])[i]) {
			if (!item->getName().compare(s)) {
				cout << item->getGroup() << " " << item->getSubgroup() << " " << item->getName() << " " << item->getTimestamp().ToString() << endl;
				return;
			}
		}
		throw invalid_argument("Item does not exist");
	}
	catch (const invalid_argument& e) { 
		cout << e.what() << endl;
	}
}

Item* Data::InsertItem(char c, int i, string s, Date d) {
	if (c < 'A' || c > 'Z' || i < 0 || i > 99 || s.empty()) return nullptr;

	Item* temp = new Item(c, i, s, d);
	if (DataStructure.count(c) > 0) {
		if ((*DataStructure[c]).count(i) > 0) {
			for (Item* item : *(*DataStructure[c])[i]) {
				if (!item->getName().compare(s)) return nullptr;
			}
			(*DataStructure[c])[i]->push_back(temp);
		}
		else {
			auto l = new list<Item*>{ temp };
			(*DataStructure[c])[i] = l;
		}
	}
	else {
		auto l = new list<Item*>{ temp };
		auto m = new map<int, list<Item*>*>{ {i, l} };
		DataStructure[c] = m;
	}
	return temp;
}

list<Item*>* Data::InsertSubgroup(char c, int i, initializer_list<Item*> items) {
	if(c < 'A' || c > 'Z' || i < 0 || i > 99 || items.size() == 0) return nullptr;
	if (DataStructure.count(c) > 0 && (*DataStructure[c]).count(i) > 0) return nullptr;

	auto l = new list<Item*>{ items };
	if(DataStructure.count(c) > 0) 
		(*DataStructure[c])[i] = l;
	else {
		auto m = new map<int, list<Item*>*>{ {i, l} };
		DataStructure[c] = m;
	}
	return l;
}

map<int, list<Item*>*>* Data::InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<Item*>> items) {
	if (c < 'A' || c > 'Z' || subgroups.size() == 0 || items.size() == 0) return nullptr;
	if (DataStructure.count(c) > 0) return nullptr;

	auto m = new map<int, list<Item*>*>;
	DataStructure[c] = m;

	initializer_list<int>::iterator i;
	initializer_list<initializer_list<Item*>>::iterator l;
	for (i = subgroups.begin(), l = items.begin(); i != subgroups.end() && l != items.end(); ++i, ++l) {
		auto ll = new list<Item*>{ *l };
		(*m)[*i] = ll;
	}
	return m;
}

bool Data::RemoveItem(char c, int i, string s) {
	Item* toRemove = GetItem(c, i, s);
	if (toRemove) {
		(*(*DataStructure[c])[i]).remove(toRemove);
		if ((*(*DataStructure[c])[i]).empty()) {
			(*DataStructure[c]).erase(i);
			if ((*DataStructure[c]).empty())
				DataStructure.erase(c);
		}
		delete toRemove;
		return true;
	}
	return false;
}

bool Data::RemoveSubgroup(char c, int i) {
	if (DataStructure.count(c) == 0 || (*DataStructure[c]).count(i) == 0) return false;
	for (auto item : *(*DataStructure[c])[i])
		delete item;
	(*DataStructure[c]).erase(i);
	if ((*DataStructure[c]).empty())
		DataStructure.erase(c);
	return true;
}

bool Data::RemoveGroup(char c) {
	if (DataStructure.count(c) == 0) return false;
	for (auto sg : *DataStructure[c])
		for (auto item : *sg.second)
			delete item;
	DataStructure.erase(c);
	return true;
}
