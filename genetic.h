#ifndef GENETIC_H
#define GENETIC_H

#include "utils.h"
#include "graph.h"
#include "heuristics.h"

using namespace std;

BINARY_LIST convert_solution(Graph &g, INT_SET solution);
BINARY_SOLUTIONS initialize_population(Graph &g, int n, bool debug=false);
int cost(BINARY_LIST solution);
BINARY_SOLUTIONS single_point_crossover(BINARY_LIST A, BINARY_LIST B);
BINARY_LIST mutation(BINARY_LIST solution, float probability, bool &m);
float fitness(BINARY_LIST vertex_cover);
BINARY_LIST roulette_wheel_selection(BINARY_SOLUTIONS population);
bool highest_fitness_comparator(const BINARY_LIST A, const BINARY_LIST B);
void describe_solutions(BINARY_SOLUTIONS list);
BINARY_LIST genetic_algorithm(Graph &g, int population_size, float crossover_fraction, float mutation_probability, int convergence, bool debug);

#endif