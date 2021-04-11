#include <iostream>
#include <list>
#include "Date.h"
#include "Item.h"
#include "Data.h"

using namespace std;

int main() {

	/* test item
	for (int i = 0; i < 20; i++) {
		Item yle = Item();
		cout << yle.getGroup() << " " << yle.getSubgroup() << " " << yle.getName() << " " << yle.getTimestamp().ToString() << endl;
	}*/
	Data yled(300);
	yled.PrintAll();
	//yled->~Data();
	//cout << yled->CountItems();
	//yled->PrintGroup('Z');
	//cout << yled->CountGroupItems('A');
	//char c;
	//int i;
	//string s;
	//cin >> c;
	//cin >> i;
	//cin.ignore();
	//getline(cin, s);
	//yled->PrintItem(c, i, s);
	//Item* yle = yled->GetItem(c, i, s);
	//cout << yle->getName();
	//yled->InsertItem(c, i, s, Date::CreateRandomDate(Date(6, 6, 2001), Date(6, 6, 2021)));
	//yled->PrintAll();
	//Item* yle = yled->InsertItem(c, i, s, Date::CreateRandomDate(Date(6, 6, 2001), Date(6, 6, 2021)));
	//cout << yle->getName() << " " << yle->getTimestamp().ToString();
	//initializer_list<Item*> items{ new Item(), new Item(), new Item() };
	//initializer_list<Item*> items1{ new Item(), new Item(), new Item() };
	//initializer_list<Item*> items2{ new Item(), new Item(), new Item() };
	//initializer_list<initializer_list<Item*>> nestitems{items1, items2};
	//initializer_list<int> subgroups{ 16, 32 };
	//map<int, list<Item*>*>* m = yled.InsertGroup(c, subgroups, nestitems);
	//list<Item*>* siko = yled.InsertSubgroup(c, i, items);
	//cout << yled.RemoveItem(c, i, s);
	//cout << yled.RemoveSubgroup(c, i);
	//cout << yled.RemoveGroup(c);
	//cout << "--------------------------------" << endl;
	//yled.PrintAll();

	return 0;
}