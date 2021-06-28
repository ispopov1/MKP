#include"Knapsack.h"
#include"Solvers.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>


using namespace std;

MultidimensionalKnapsack GenerateKnapsack(int n, int m, int a_max, int c_max) {
	vector<Item> items(n);
	vector<int> b(m);
	for (int j = 0; j < n; j++)
	{
		int x = rand() % (c_max)+1;
		items[j].addValue(x);
		items[j].id = j;
	}
	for (int i = 0; i < m; i++)
	{
		int sum = 0;
		int max = 0;
		for (int j = 0; j < n; j++)
		{
			int x = rand() % (a_max)+1;
			sum += x;
			if (x > max) {
				max = x;
			}
			items[j].addConstraight(x);
		}
		b[i] = rand() % (sum - max) + max;
	}
	return MultidimensionalKnapsack(n, m, items, b);
}

int max(int a, int b) { return (a > b) ? a : b; }

// Returns the maximum value that
// can be put in a knapsack of capacity W
int knapSack(int W, vector<int> wt, vector<int> val, int n)
{

	// Base Case
	if (n == 0 || W == 0)
		return 0;

	// If weight of the nth item is more
	// than Knapsack capacity W, then
	// this item cannot be included
	// in the optimal solution
	if (wt[n - 1] > W)
		return knapSack(W, wt, val, n - 1);

	// Return the maximum of two cases:
	// (1) nth item included
	// (2) not included
	else
		return max(
			val[n - 1]
			+ knapSack(W - wt[n - 1],
				wt, val, n - 1),
			knapSack(W, wt, val, n - 1));
}

int main(int argc, char* argv[])
{
	// Shih sapmle
	/*vector<Item> items;
	items.push_back(Item{ 84,{20,41} });
	items.push_back({ 34,{12,51} });
	items.push_back({ 31,{7,24} });
	items.push_back({ 14,{75,40} });
	items.push_back({ 67,{93,84} });
	items.push_back({ 65,{21,70} });
	items.push_back({ 86,{75,34} });
	items.push_back({ 98,{67,41} });
	items.push_back({ 50,{34,49} });
	items.push_back({ 7,{28,27} });
	MultidimensionalKnapsack m = { 10,2,items,{190,250} };
	auto start_time = chrono::steady_clock::now();
	double x = shihsolve({ 10,2,items,{190,250} });
	auto finish_time = chrono::steady_clock::now();
	auto microseconds = chrono::duration_cast<chrono::microseconds>(finish_time - start_time).count();
	cout << x << endl << microseconds << endl;*/
	MultidimensionalKnapsack m = GenerateKnapsack(20, 10, 40, 40);

	auto start_time = chrono::steady_clock::now();
	auto x = OrderedSetOfNodesSolve(m);
	auto  finish_time = chrono::steady_clock::now();
	auto microseconds = chrono::duration_cast<chrono::microseconds>(finish_time - start_time).count();
	cout << "Ordered set of nodes solution:\n";
	cout << "Answer: " << x << ", Time: " << microseconds << endl;

	start_time = chrono::steady_clock::now();
	x = OrderedSetOfNodesParallelSolve(m);
	finish_time = chrono::steady_clock::now();
	microseconds = chrono::duration_cast<chrono::microseconds>(finish_time - start_time).count();
	cout << "Ordered set of nodes parallel solution:\n";
	cout << "Answer: " << x << ", Time: " << microseconds << endl;

	start_time = chrono::steady_clock::now();
	x = BestSolutionSavingSolve(m);
	finish_time = chrono::steady_clock::now();
	microseconds = chrono::duration_cast<chrono::microseconds>(finish_time - start_time).count();
	cout << "Best node saving solution:\n";
	cout << "Answer: " << x << ", Time: " << microseconds << endl;

	start_time = chrono::steady_clock::now();
	x = BestSolutionSavingParallelSolve(m);
	finish_time = chrono::steady_clock::now();
	microseconds = chrono::duration_cast<chrono::microseconds>(finish_time - start_time).count();
	cout << "Best node saving parallel solution:\n";
	cout << "Answer: " << x << ", Time: " << microseconds << endl;
}