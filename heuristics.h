#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "utils.h"
#include "graph.h"

using namespace std;

bool verify_vertex_cover(Graph &g, INT_SET cover);

/* simple heuristic */
// INT_SET most_neighbors_first(Graph &g, int alpha=5, int seed=1, bool debug_mode=false);
// INT_SET random_multistart_most_neighbors_first(Graph &g, int iterations, int alpha=5, bool debug_mode=false);
INT_SET greedy(Graph &g, int seed=0, bool debug_mode=false);
INT_SET rm_greedy(Graph &g, int iterations, bool debug_mode=false);
vector<INT_SET> rm_greedy_all(Graph &g, int iterations, bool debug_mode=false);
INT_SET semi_greedy(Graph &g, int seed=0, float alpha=1, bool debug_mode=false);

/* local search */
// INT_PAIR_LIST loss(Graph &g, INT_SET Vc, int v);
INT_PAIR_LIST loss(Graph &g, INT_SET Vc, INT_SET V);
INT_SET first_improving(Graph &g, INT_SET solution, int max_it, int seed, bool debug_mode=false);
INT_SET local_search(Graph &g, string type, int max_it, int max_it_1st, int &search_iterations, INT_SET initial_solution=INT_SET_NULL, bool debug_mode=false);
INT_SET rm_local_search(Graph &g, string type, int it_rm, int max_it, int max_it_1st, int &improvement, bool debug_mode);
INT_SET repair(Graph &g, INT_SET incomplete_Vc, int seed);
INT_SET grasp(Graph &g, float alpha, int max_time_ms, int max_iterations, bool debug_mode);

#endif