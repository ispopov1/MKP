#include"Knapsack.h"
#include"ShihAlgorithm.h"
#include"Solvers.h"
#include <algorithm>
#include <set>
#include <omp.h>

using namespace std;

double BestSolutionSavingParallelSolve(MultidimensionalKnapsack knapsack) {
	int n = knapsack.getN();
	int m = knapsack.getM();
	vector<Item> items = knapsack.getItems();
	vector<int> b = knapsack.getB();
	vector<vector<int>> indices = GenerateAuxiliaryIndices(knapsack);
	// I - set of items guaranteed used in current node, E - guaranteed not used items.
	set<Item> I, E;
	// Nodes which can become new best solution.
	vector<Node> availableNodes;
	// Calculating bound at the starting node.
	pair<double, int> x = GetBound(knapsack, indices, I, E);
	Node startNode;
	Node bestNode;
	startNode.bound = x.first;
	startNode.branchNum = x.second;
	startNode.E = E;
	startNode.I = I;
	int best = 0;
	startNode.nodeID = 0;
	availableNodes.push_back(startNode);
	while (true) {
		if ((availableNodes.size() == 1) && (best > 0) && (availableNodes[0].branchNum == -1)) {
			break;
		}
		int countOfNodes = availableNodes.size();
		// Saves best solution for each thread.
		vector<int> bests(omp_get_max_threads());
		vector<Node> bestNodes(omp_get_max_threads());
		availableNodes.resize(availableNodes.size() * 2);
#pragma omp parallel for
		for (int i = 0; i < countOfNodes; i++)
		{
			int j = omp_get_thread_num();
			Node node = availableNodes[i];
			// Branching condition.
			if (node.branchNum != -1)
			{
				availableNodes[i] = Node();
				set<Item> I, E;
				Node left_node, right_node;
				bool isLeft = false;
				bool isRight = false;
				I = node.I;
				E = node.E;
				// Item num to branch at.
				int num = node.branchNum;
				// Suppose branching item not included.
				E.insert(items[num]);
				if (isFeasible(knapsack, I, E)) {
					pair<double, int> b = GetBound(knapsack, indices, I, E);
					left_node.bound = b.first;
					left_node.branchNum = b.second;
					left_node.I = I;
					left_node.E = E;
					left_node.nodeID = node.nodeID * 2 + 1;
					if (left_node.bound >= bests[j]) {
						availableNodes[i] = left_node;
					}
					if (left_node.branchNum == -1) {
						if (left_node.bound > bests[j]) {
							bests[j] = left_node.bound;
							bestNodes[j] = left_node;
						}
					}
				}
				// Suppose branching item  included.
				E.erase(items[num]);
				I.insert(items[num]);
				if (isFeasible(knapsack, I, E)) {
					pair<double, int> b = GetBound(knapsack, indices, I, E);
					right_node.bound = b.first;
					right_node.branchNum = b.second;
					right_node.I = I;
					right_node.E = E;
					right_node.nodeID = node.nodeID * 2 + 2;;
					if (right_node.bound >= bests[j]) {
						availableNodes[i + countOfNodes] = right_node;
					}
					if (right_node.branchNum == -1) {
						if (right_node.bound > bests[j]) {
							bests[j] = right_node.bound;
							bestNodes[j] = right_node;
						}
					}

				}
			}
			else {
				// Compare integer solution to best solution.
				if (node.bound > bests[j] || node == bestNodes[j]) {
					bests[j] = node.bound;
					bestNodes[j] = node;
				}
			}

		}
		// Finding best solution for whole vector.
		for (size_t i = 0; i < omp_get_max_threads(); i++)
		{
			if (bests[i] > best) {
				best = bests[i];
				bestNode = bestNodes[i];
			}
		}
		// Removing nodes with bound less then best solution value.
		vector<Node>::iterator it;
		for (it = availableNodes.begin(); it != availableNodes.end();)
		{
			if (it->bound < best) {
				it = availableNodes.erase(it);
			}
			else {
				if (it->bound == best) {
					if (it->nodeID != bestNode.nodeID) {
						it = availableNodes.erase(it);
					}
					else {
						it++;
					}
				}
				else {
					it++;
				}
			}
		}

	}
	return (availableNodes[0]).bound;
}
