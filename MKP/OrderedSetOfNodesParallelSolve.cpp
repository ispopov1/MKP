#include"Knapsack.h"
#include"ShihAlgorithm.h"
#include"Solvers.h"
#include <algorithm>
#include <set>
#include <omp.h>

using namespace std;

double OrderedSetOfNodesParallelSolve(MultidimensionalKnapsack knapsack) {
	// Knapsack capacity
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
		if ((*(availableNodes.begin())).branchNum == -1) {
			break;
		}
		set<Node> iteratorNotes = availableNodes;
#pragma omp parallel for
		for (int i = 0; i < iteratorNotes.size(); i++)
		{
			auto it = iteratorNotes.begin();
			advance(it, i);
			if (it == iteratorNotes.end()) {
				int g = 15;
			}
			Node node = *it;
			if (node.branchNum != -1) {
				set<Item> I, E;
				Node left_node, right_node;
				bool isLeft = false;
				bool isRight = false;
				I = node.I;
				E = node.E;
				int num = node.branchNum;
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
#pragma omp critical
				{
					availableNodes.erase(node);
					if (isRight) {
						availableNodes.insert(right_node);
					}
					if (isLeft) {
						availableNodes.insert(left_node);
					}
				}
			}

		}
	}
	return (*(availableNodes.begin())).bound;
}