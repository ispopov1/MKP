#include"Knapsack.h"
#include <iostream>
#include <vector>

using namespace std;

Item::Item(){}

Item::Item(int c, vector<int> a)
	: c(c), a(a) {}

void Item::addConstraight(int x) {
	a.push_back(x);
}
vector<int> Item::getConstraights() {
	return a;
}
void Item::addValue(int x) {
	c = x;
}
int Item::getValue() {
	return c;
}

bool Item::operator<(const Item& rhs) const
{
	if (c == rhs.c) {
		return id < rhs.id;
	}
	return c < rhs.c;
}


MultidimensionalKnapsack::MultidimensionalKnapsack(int n, int m, vector<Item> items, vector<int> b)
	: n(n), m(m), items(items), b(b)
{ }
MultidimensionalKnapsack::MultidimensionalKnapsack()
{ }
int MultidimensionalKnapsack::getM() {
	return m;
}
int MultidimensionalKnapsack::getN() {
	return n;
}
vector<Item> MultidimensionalKnapsack::getItems() {
	return items;
}
vector<int> MultidimensionalKnapsack::getB() {
	return b;
}
