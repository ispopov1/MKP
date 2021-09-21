#ifndef SOLVERS_H
#define SOLVERS_H

#include"Knapsack.h"
#include <algorithm>
#include <set>

using namespace std;

double BestSolutionSavingParallelSolve(MultidimensionalKnapsack knapsack);
double BestSolutionSavingSolve(MultidimensionalKnapsack knapsack);
double OrderedSetOfNodesParallelSolve(MultidimensionalKnapsack knapsack, int countOfThreads);
double OrderedSetOfNodesSolve(MultidimensionalKnapsack knapsack);

#endif
