#ifndef GENETIC_H
#define GENETIC_H

#include "utils.h"
#include "graph.h"
#include "heuristics.h"

using namespace std;

BINARY_LIST convert_solution(Graph &g, INT_SET solution);
BINARY_SOLUTIONS initialize_population(Graph &g, int n);
int cost(BINARY_LIST solution);
BINARY_SOLUTIONS single_point_crossover(BINARY_LIST A, BINARY_LIST B);
BINARY_LIST mutation(BINARY_LIST solution, float probability);

#endif