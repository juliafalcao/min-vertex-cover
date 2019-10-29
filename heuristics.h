#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "utils.h"
#include "graph.h"

using namespace std;

#define INT_MAX 2147483647

bool verify_vertex_cover(Graph &g, INT_SET cover);

/* simple heuristic */
// INT_SET most_neighbors_first(Graph &g, int alpha=5, int seed=1, bool debug_mode=false);
// INT_SET random_multistart_most_neighbors_first(Graph &g, int iterations, int alpha=5, bool debug_mode=false);
INT_SET simple_greedy(Graph &g, int seed=0, bool debug_mode=false);
INT_SET random_multistart_simple_greedy(Graph &g, int iterations, bool debug_mode=false);

/* local search */
// INT_PAIR_LIST loss(Graph &g, INT_SET Vc, int v);
INT_PAIR_LIST loss(Graph &g, INT_SET Vc, INT_SET V);
INT_SET first_improving(Graph &g, INT_SET solution, int max_it, int seed, bool debug_mode=false);
INT_SET local_search(Graph &g, int max_it_first_improving, int &search_iterations, bool debug_mode=false);

#endif