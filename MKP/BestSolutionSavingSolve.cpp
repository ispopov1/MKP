#include"Knapsack.h"
#include"ShihAlgorithm.h"
#include"Solvers.h"
#include <algorithm>
#include <set>
#include <omp.h>

using namespace std;

double BestSolutionSavingSolve(MultidimensionalKnapsack knapsack) {
	// Knapsack capacity
	int n = knapsack.getN();
	int m = knapsack.getM();
	vector<Item> items = knapsack.getItems();
	vector<int> b = knapsack.getB();
	vector<vector<int>> indices = GenerateAuxiliaryIndices(knapsack);
	// I - set of items guaranteed used in current node, E - guaranteed not used items.
	set<Item> I, E;
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
		availableNodes.resize(availableNodes.size() * 2);
		for (int i = 0; i < countOfNodes; i++)
		{
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
				// branch item number
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
					if (left_node.bound >= best) {
						availableNodes[i] = left_node;
					}
					if (left_node.branchNum == -1) {
						if (left_node.bound > best) {
							best = left_node.bound;
							bestNode = left_node;
						}
					}
				}
				// Suppose branching item included.
				E.erase(items[num]);
				I.insert(items[num]);
				if (isFeasible(knapsack, I, E)) {
					pair<double, int> b = GetBound(knapsack, indices, I, E);
					right_node.bound = b.first;
					right_node.branchNum = b.second;
					right_node.I = I;
					right_node.E = E;
					right_node.nodeID = node.nodeID * 2 + 2;;
					if (right_node.bound >= best) {
						availableNodes[i + countOfNodes] = right_node;
					}
					if (right_node.branchNum == -1) {
						if (right_node.bound > best) {
							best = right_node.bound;
							bestNode = right_node;
						}
					}

				}
			}
			else {
				// Compare integer solution to best solution.
				if (node.bound > best || node == bestNode) {
					best = node.bound;
					bestNode = node;
				}
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