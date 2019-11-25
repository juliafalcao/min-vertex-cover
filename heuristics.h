#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "utils.h"
#include "graph.h"

using namespace std;

/* verification function */
bool verify_vertex_cover(Graph &g, INT_SET cover);

/* simple greedy heuristics */
INT_SET greedy(Graph &g, int seed=0, bool debug=false);
INT_SET rm_greedy(Graph &g, int iterations, bool debug=false);
vector<INT_SET> rm_greedy_all(Graph &g, int iterations, bool debug=false);
INT_SET semi_greedy(Graph &g, int seed=0, float alpha=1, bool debug=false);

/* local search */
// INT_PAIR_LIST loss(Graph &g, INT_SET Vc, int v);
INT_PAIR_LIST loss(Graph &g, INT_SET Vc, INT_SET V);
INT_SET first_improving(Graph &g, INT_SET solution, int max_it, int seed, bool debug=false);
INT_SET local_search(Graph &g, int max_it, int max_it_1st, INT_SET initial_solution=INT_SET_NULL, bool debug=false);
INT_SET rm_local_search(Graph &g, string type, int it_rm, int max_it, int max_it_1st, int &improvement, bool debug);

/* grasp */
INT_SET repair(Graph &g, INT_SET incomplete_Vc, int seed);
INT_SET grasp(Graph &g, float alpha, int max_time_ms, int max_iterations, int target=-1, bool debug=false);

/* grasp + path relinking */
SOLUTION_SET restricted_neighborhood(Graph &g, INT_SET initial_solution, INT_SET guiding_solution);
float similarity(Graph &g, INT_SET A, INT_SET B);
INT_SET best_solution(Graph &g, SOLUTION_SET solutions, INT_SET closer_to, int &tie);
INT_SET forward_path_relinking(Graph &g, INT_SET initial_solution, INT_SET guiding_solution, bool debug=false);
SOLUTION_SET update_elite_set(INT_SET new_solution, SOLUTION_SET elite_set, int max_size, int delta_threshold=0, bool debug=false);
INT_SET grasp_pr(Graph &g, float alpha, int max_time_ms, int max_iterations, int max_elite, bool debug=false);

#endif