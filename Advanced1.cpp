#include <iostream>
#include <list>
#include <string>
#include "Date.h"
#include "Item.h"
#include "Data.h"

using namespace std;

int main() {
	
	char c;
	int i;
	string s;

	cout << "[1] Create 300 items, print them and count items\n";
	Data yled(300);
	yled.PrintAll();
	cout << "Number of items: " << yled.CountItems() << endl;

	cout << "[2] Select group, print and count items\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	yled.PrintGroup(c);
	cout << "Number of items in group " << yled.CountGroupItems(c) << endl;

	cout << "[3] Select subgroup and print it by names, by dates and then count items\n";
	cout << "Select subgroup with 2+ items\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cout << "\nBy names\n";
	yled.PrintSubgroupByNames(c, i);
	cout << "\nBy dates\n";
	yled.PrintSubgroupByDates(c, i);
	cout << "Number of items in group " << yled.CountSubgroupItems(c, i) << endl;

	cout << "[4] Select subgroup and print it by names, by dates and then count items\n";
	cout << "Select subgroup with 1 item\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cout << "\nBy names\n";
	yled.PrintSubgroupByNames(c, i);
	cout << "\nBy dates\n";
	yled.PrintSubgroupByDates(c, i);
	cout << "Number of items in group " << yled.CountSubgroupItems(c, i) << endl;


	cout << "[5] Select subgroup and print it by names, by dates and then count items\n";
	cout << "Select non-existing subgroup\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cout << "\nBy names\n";
	yled.PrintSubgroupByNames(c, i);
	cout << "\nBy dates\n";
	yled.PrintSubgroupByDates(c, i);
	cout << "Number of items in group " << yled.CountSubgroupItems(c, i) << endl;
	
	cout << "[6] Print existing and non-existing item\n";
	cout << "Select existing item\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	yled.PrintItem(c, i, s);
	cout << "Select non-existing item\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	yled.PrintItem(c, i, s);

	cout << "[7] Create 30 items and print\n";
	Data d1(30);
	d1.PrintAll();

	cout << "[8] Print and count non-existing group items\n";
	cout << "Select non-existing group (A-Z): ";
	cin >> c;
	d1.PrintGroup(c);
	d1.CountGroupItems(c);
	
	cout << "[9] Insert items and check the results\n";
	cout << "[9a]Select existing group and subgroup\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d1.InsertItem(c, i, s, Date::CreateRandomDate(Date(6, 6, 2001), Date(6, 6, 2021)));
	cout << "[9b] Select existing group and non-existing subgroup\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d1.InsertItem(c, i, s, Date::CreateRandomDate(Date(6, 6, 2001), Date(6, 6, 2021)));
	cout << "[9c] Select non-existing group\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d1.InsertItem(c, i, s, Date::CreateRandomDate(Date(6, 6, 2001), Date(6, 6, 2021)));
	cout << "[9d] Select existing item\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d1.InsertItem(c, i, s, Date::CreateRandomDate(Date(6, 6, 2001), Date(6, 6, 2021)));
	cout << "[9e] Check results\n";
	d1.PrintAll();
	
	cout << "[10] Insert subgroups and check results\n";
	cout << "[10a] Select existing group\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	initializer_list<Item*> items1{ new Item(), new Item(), new Item() };
	d1.InsertSubgroup(c, i, items1);
	cout << "[10b] Select non-existing group\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	initializer_list<Item*> items2{ new Item(), new Item(), new Item() };
	d1.InsertSubgroup(c, i, items2);
	cout << "[10c] Select existing subgroup\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	initializer_list<Item*> items3{ new Item(), new Item(), new Item() };
	d1.InsertSubgroup(c, i, items3);
	cout << "[10d] Check results\n";
	d1.PrintAll();
	
	cout << "[11] Insert groups\n";
	cout << "[11a] Select non-existing group\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	initializer_list<Item*> itemsA{ new Item(), new Item(), new Item() };
	initializer_list<Item*> itemsB{ new Item(), new Item(), new Item() };
	initializer_list<initializer_list<Item*>> nestitems1{ itemsA, itemsB };
	initializer_list<int> subgroups1{ 16, 32 };
	d1.InsertGroup(c, subgroups1, nestitems1);
	cout << "[11b] Select existing group\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	initializer_list<Item*> itemsC{ new Item(), new Item(), new Item() };
	initializer_list<Item*> itemsD{ new Item(), new Item(), new Item() };
	initializer_list<initializer_list<Item*>> nestitems2{ itemsA, itemsB };
	initializer_list<int> subgroups2{ 16, 32 };
	d1.InsertGroup(c, subgroups2, nestitems2);
	cout << "[11c] Check Results\n";
	d1.PrintAll();
	
	cout << "[12] Create 100 items and print\n";
	Data d2(100);
	d2.PrintAll();
	
	cout << "[13] Remove Items and check results\n";
	cout << "[13a] Select subgroup with 2 items\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d2.RemoveItem(c, i, s);
	cout << "[13b] Select group with multiple subgroups and subgroup with 1 item\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d2.RemoveItem(c, i, s);
	cout << "[13c] Select group with 1 subgroup and item\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d2.RemoveItem(c, i, s);
	cout << "[13d] Select non-existing item\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	cin.ignore();
	cout << "Select name: ";
	getline(cin, s);
	d2.RemoveItem(c, i, s);
	cout << "[13e] Check results\n";
	d2.PrintAll();

	cout << "[14] Remove subgroups and check results\n";
	cout << "[14a] Select group with several subgroups\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	d2.RemoveSubgroup(c, i);
	cout << "[14b] Select group with 1 subgroup\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	d2.RemoveSubgroup(c, i);
	cout << "[14c] Select non-existing subgroup\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	cout << "Select subgroup (0-99): ";
	cin >> i;
	d2.RemoveSubgroup(c, i);
	cout << "[14d] Check results\n";
	d2.PrintAll();
	
	cout << "[15] Remove groups and check results\n";
	cout << "[15a] Select existing group\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	d2.RemoveGroup(c);
	cout << "[15b] Select non-existing group\n";
	cout << "Select group (A-Z): ";
	cin >> c;
	d2.RemoveGroup(c);
	cout << "[15c] Check Results\n";
	d2.PrintAll();
	cout << "\n[--------Testing Finished--------]\n";

	return 0;
}