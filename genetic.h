#ifndef GENETIC_H
#define GENETIC_H

#include "utils.h"
#include "graph.h"

using namespace std;

BINARY_LIST convert_solution(Graph &g, INT_SET solution);
BINARY_LIST binary_greedy(Graph &g, int seed, bool debug=false);

SOLUTIONS_LIST initialize_population(Graph &g, int n, bool debug=false);
int cost(BINARY_LIST solution);
SOLUTIONS_LIST single_point_crossover(BINARY_LIST A, BINARY_LIST B, bool debug=false);
BINARY_LIST mutation(BINARY_LIST solution, float probability, bool &m);
float fitness(BINARY_LIST vertex_cover);
BINARY_LIST roulette_wheel_selection(SOLUTIONS_LIST population);
bool highest_fitness_comparator(const BINARY_LIST &A, const BINARY_LIST &B);
void describe_solutions(SOLUTIONS_LIST list);
BINARY_LIST genetic_algorithm(Graph &g, int np, float crossover, float mutation_probability, int stability, bool debug);
bool verify_solution(Graph &g, BINARY_LIST solution);
SOLUTIONS_LIST diversify(Graph &g, BINARY_LIST solution, int amount, bool debug);

#endif