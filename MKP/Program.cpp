#include"Knapsack.h"
#include"Solvers.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

vector<MultidimensionalKnapsack> readInstances(string path) {
	ifstream fin(path);
	// Count of samples.
	int count;
	fin >> count;
	vector<MultidimensionalKnapsack> problems(count);
	for (size_t i = 0; i < count; i++)
	{
		int n, m, o;
		fin >> n >> m >> o;
		vector<Item> items(n);
		vector<int> b(m);
		for (size_t j = 0; j < n; j++)
		{
			int v;
			fin >> v;
			items[j].addValue(v);
			items[j].id = j;
		}
		for (size_t j = 0; j < n; j++)
		{
			for (size_t k = 0; k < m; k++)
			{
				int w;
				fin >> w;
				items[j].addConstraight(w);
			}
		}
		for (size_t j = 0; j < m; j++)
		{
			int x;
			fin >> x;
			b[j] = x;
		}
		problems[i] = MultidimensionalKnapsack(n, m, items, b);
	}
	return problems;
}

int main(int argc, char* argv[])
{
	ofstream fout;
	fout.open("../experiment_result/10x100x050(All).txt");
	vector<MultidimensionalKnapsack> problems = readInstances("../instances/OR10x100/OR10x100.dat");
	fout << "n = 100, m = 10, a = 0,50" << endl << endl;
	double avg = 0;

	fout << "SA12" << endl;
	for (size_t i = 10; i < 20; i++)
	{
		auto start_time = chrono::steady_clock::now();
		auto x = OrderedSetOfNodesParallelSolve(problems[i], 12);
		auto  finish_time = chrono::steady_clock::now();
		auto microseconds = chrono::duration_cast<chrono::milliseconds>(finish_time - start_time).count();
		fout << (double)microseconds / 1000 << " ";
		avg += (double)microseconds / 1000;
		fout.flush();
	}
	fout << endl;
	avg /= 10;
	fout << "AVG = " << avg << endl;
	avg = 0;
	fout << "SA1" << endl;
	for (size_t i = 10; i < 20; i++)
	{
		auto start_time = chrono::steady_clock::now();
		auto x = OrderedSetOfNodesSolve(problems[i]);
		auto  finish_time = chrono::steady_clock::now();
		auto microseconds = chrono::duration_cast<chrono::milliseconds>(finish_time - start_time).count();
		fout << (double)microseconds / 1000 << " ";
		avg += (double)microseconds / 1000;
		fout.flush();
	}
	fout << endl;
	avg /= 10;
	fout << "AVG = " << avg << endl;
	avg = 0;
	fout << "BSS" << endl;
	for (size_t i = 10; i < 20; i++)
	{
		auto start_time = chrono::steady_clock::now();
		auto x = BestSolutionSavingSolve(problems[i]);
		auto  finish_time = chrono::steady_clock::now();
		auto microseconds = chrono::duration_cast<chrono::milliseconds>(finish_time - start_time).count();
		fout << (double)microseconds / 1000 << " ";
		avg += (double)microseconds / 1000;
		fout.flush();
	}
	fout << endl;
	avg /= 10;
	fout << "AVG = " << avg << endl;
	avg = 0;
	fout << "BSSP" << endl;
	for (size_t i = 10; i < 20; i++)
	{
		auto start_time = chrono::steady_clock::now();
		auto x = BestSolutionSavingParallelSolve(problems[i]);
		auto  finish_time = chrono::steady_clock::now();
		auto microseconds = chrono::duration_cast<chrono::milliseconds>(finish_time - start_time).count();
		fout << (double)microseconds / 1000 << " ";
		avg += (double)microseconds / 1000;
		fout.flush();
	}
	fout << endl;
	avg /= 10;
	fout << "AVG = " << avg << endl;
	fout.flush();
	fout.close();
}