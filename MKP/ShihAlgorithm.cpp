#include"Knapsack.h"
#include"ShihAlgorithm.h"
#include <algorithm>
#include <set>
#include <omp.h>

using namespace std;



bool Node::operator<(const Node& rhs) const
{
	if (bound == rhs.bound) {
		return nodeID < rhs.nodeID;
	}
	return bound > rhs.bound;
}
bool Node::operator==(const Node& rhs) const
{
	return nodeID == rhs.nodeID;
}
bool Node::operator!=(const Node& rhs) const
{
	return nodeID != rhs.nodeID;
}

// Generates auxiliary indices for each item. 
vector<vector<int>> GenerateAuxiliaryIndices(MultidimensionalKnapsack knapsack) {
	// indices[i][j] - j-th item order in sorted sequence of items by item_value/item_weight[i] 
	// ratio, where item_weight[i] is item weight in i-th constraight.
	vector<vector<int>> indices;
	int n = knapsack.getN();
	int m = knapsack.getM();
	vector<Item> items = knapsack.getItems();
	// List of constraights values.
	vector<int> b = knapsack.getB();
	for (int i = 0; i < m; i++)
	{
		vector<pair<double, int>> ratio;
		for (int j = 0; j < n; j++)
		{
			double q = (double)items[j].getValue() / items[j].getConstraights()[i];
			ratio.push_back(make_pair(q, j));
		}
		sort(ratio.begin(), ratio.end());
		reverse(ratio.begin(), ratio.end());
		vector<int> ratio_order;
		for (int j = 0; j < n; j++)
		{
			ratio_order.push_back(ratio[j].second);
		}
		indices.push_back(ratio_order);
	}
	return indices;
}

pair<double, int> GetBound(
	MultidimensionalKnapsack knapsack, vector<vector<int>> indices, set<Item> I, set<Item> E) {
	int n = knapsack.getN();
	int m = knapsack.getM();
	vector<Item> items = knapsack.getItems();
	vector<int> b = knapsack.getB();
	double min = DBL_MAX;
	int minIndex = 9999;
	vector<pair<double, int>> bounds(m);
	for (int i = 0; i < m; i++)
	{
		int index = -1;
		// i-th constraight current weight using.
		double weight = 0;
		vector<double> seq(n);
		for (int j = 0; j < n; j++)
		{
			if (I.count(items[j]) == 1) {
				weight += items[j].getConstraights()[i];
				seq[j] = 1;
			}
		}
		for (int j = 0; j < n; j++)
		{
			if ((I.count(items[indices[i][j]]) == 1) || (E.count(items[indices[i][j]]) == 1)) {
				continue;
			}
			if (weight + items[indices[i][j]].getConstraights()[i] <= b[i]) {
				weight += items[indices[i][j]].getConstraights()[i];
				seq[indices[i][j]] = 1;
			}
			else {
				seq[indices[i][j]] = (b[i] - weight) / items[indices[i][j]].getConstraights()[i];
				weight = b[i];
				index = indices[i][j];
			}
			if (weight == b[i]) {
				break;
			}
		}
		// i-th constraight max value.
		bounds[i].second = index;
		for (int j = 0; j < n; j++)
		{
			bounds[i].first += seq[j] * items[j].getValue();
		}
	}

	for (auto bound : bounds)
	{
		if (bound.first < min) {
			min = bound.first;
			minIndex = bound.second;
		}
	}
	return make_pair(min, minIndex);
}

// Checks if solution at current node is possible.
bool isFeasible(MultidimensionalKnapsack knapsack, set<Item> I, set<Item> E) {
	int n = knapsack.getN();
	int m = knapsack.getM();
	vector<Item> items = knapsack.getItems();
	vector<int> b = knapsack.getB();
	vector<double> minItems(n);
	bool feasible = true;
	for (int i = 0; i < m; i++)
	{
		double weight = 0;
		for (int j = 0; j < n; j++)
		{
			if (I.count(items[j]) == 1) {
				weight += items[j].getConstraights()[i];
			}
		}
		if (weight > b[i]) {
			feasible = false;
			break;
		}
	}
	return feasible;
}



