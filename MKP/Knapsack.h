#ifndef KNAPSACK_H
#define KNAPSACK_H
#include <iostream>
#include <vector>

using namespace std;

class Item {
public:
	Item();
	Item(int c, vector<int> a);
	void addConstraight(int x);
	vector<int> getConstraights();
	void addValue(int x);
	int getValue();
	int id;
	bool operator<(const Item& rhs) const;
private:
	int c; // item profit
	vector<int> a;// a[i] = item weight in the i-th knapsack constraight

};

class MultidimensionalKnapsack {
public:
	MultidimensionalKnapsack(int n, int m, vector<Item> items, vector<int> b);
	MultidimensionalKnapsack();
	int getM();
	int getN();
	vector<Item> getItems();
	vector<int> getB();
private:
	int n; // number of items
	int m; // number of knapsack constraights
	vector<Item> items; // items[i] - i-th item
	vector<int> b; // b[i] - capacity of i-th knapsack constraight

};
#endif 
