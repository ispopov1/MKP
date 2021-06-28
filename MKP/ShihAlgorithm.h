#ifndef SHIH_H
#define SHIH_H

#include"Knapsack.h"
#include <algorithm>
#include <set>

using namespace std;

class Node {
public:
	double bound;
	int branchNum;
	long long nodeID;
	set<Item> I, E;

	bool operator<(const Node& rhs) const;
	bool operator==(const Node& rhs) const;
	bool operator!=(const Node& rhs) const;
};

vector<vector<int>> GenerateAuxiliaryIndices(MultidimensionalKnapsack knapsack);
pair<double, int> GetBound(
	MultidimensionalKnapsack knapsack, vector<vector<int>> indices, set<Item> I, set<Item> E);
bool isFeasible(MultidimensionalKnapsack knapsack, set<Item> I, set<Item> E);

#endif