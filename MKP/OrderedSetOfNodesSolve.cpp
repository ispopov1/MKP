#include"Knapsack.h"
#include"ShihAlgorithm.h"
#include"Solvers.h"
#include <algorithm>
#include <set>
#include <omp.h>

using namespace std;

double OrderedSetOfNodesSolve(MultidimensionalKnapsack knapsack) {
	int n = knapsack.getN();
	int m = knapsack.getM();
	vector<Item> items = knapsack.getItems();
	vector<int> b = knapsack.getB();
	vector<vector<int>> indices = GenerateAuxiliaryIndices(knapsack);
	set<Item> I, E;
	set<Node> availableNodes;
	pair<double, int> x = GetBound(knapsack, indices, I, E);
	Node startNode;
	startNode.bound = x.first;
	startNode.branchNum = x.second;
	startNode.E = E;
	startNode.I = I;
	startNode.nodeID = 0;
	availableNodes.insert(startNode);
	while (true) {
		// Checks if best solution is integer.
		if ((*(availableNodes.begin())).branchNum == -1) {
			break;
		}
		set<Node> iteratorNotes = availableNodes;
		Node node = *(availableNodes.begin());
		// Node branching.
		if (node.branchNum != -1) {
			set<Item> I, E;
			Node left_node, right_node;
			bool isLeft = false;
			bool isRight = false;
			I = node.I;
			E = node.E;
			int num = node.branchNum;
			// Left child node.
			E.insert(items[num]);
			if (isFeasible(knapsack, I, E)) {
				pair<double, int> b = GetBound(knapsack, indices, I, E);
				left_node.bound = b.first;
				left_node.branchNum = b.second;
				left_node.I = I;
				left_node.E = E;
				left_node.nodeID = node.nodeID * 2 + 1;
				isLeft = true;
			}
			E.erase(items[num]);
			// Right child node
			I.insert(items[num]);
			if (isFeasible(knapsack, I, E)) {
				pair<double, int> b = GetBound(knapsack, indices, I, E);
				right_node.bound = b.first;
				right_node.branchNum = b.second;
				right_node.I = I;
				right_node.E = E;
				left_node.nodeID = node.nodeID * 2 + 2;;
				isRight = true;

			}
			// Removes parent node from available nodes.
			availableNodes.erase(node);
			// Adds children nodes if they are feasible.
			if (isRight) {
				availableNodes.insert(right_node);
			}
			if (isLeft) {
				availableNodes.insert(left_node);
			}

		}
	}
	return (*(availableNodes.begin())).bound;
}