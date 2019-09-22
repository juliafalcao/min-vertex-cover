#include "heuristics.h"


/*
verification algorithm
*/
bool verify_vertex_cover(Graph &g, INT_SET cover) {
	for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
		if (find(cover.begin(), cover.end(), it->first) == cover.end() 
		&& find(cover.begin(), cover.end(), it->second) == cover.end()) {
			// neither vertex is in vertex cover: edge isn't covered
			return false;
		}
	}

	return true;
}


/*
first constructive heuristic: greedy adaptive?
alpha = number of vertices with most neighbors to randomly choose from
*/
INT_SET most_neighbors_first(Graph &g, int alpha, int seed, bool debug_mode) {
	if (alpha < 2) error("Alpha value has to be at least 2 (vertices).");

	ADJ_PAIR_LIST adjc = g.get_adj_copy(); // to manipulate without altering g

	sort(adjc.begin(), adjc.end(), most_values_comparator); // sort by adj lists with most values first

	int total_edges = g.get_m();
	int covered_edges = 0;
	int newly_covered_edges = 0;
	int new_edges = 0;
	int remaining_vertices = 0;

	int v_index = 0;
	srand(seed);

	INT_SET Vc; // vertex cover!
	int v; // current vertex
	INT_LIST v_adj; // current vertex's adjacencies list
	INT_LIST s_v_adj; // subsequent vertices' adjacencies lists

	while (adjc.size() > 0) { // main loop

		if (adjc.size() > alpha) {
			// choose random vertex amongst the <alpha> first in adjc
			v_index = rand() % alpha;
			if (debug_mode) printf("Random (alpha=%d) chose %d-th vertex.\n", alpha, v_index);
		} else {
			v_index = 0;
			if (debug_mode) printf("List smaller than %d elements; using first.\n", alpha);
		}

		v = adjc[v_index].first; // chosen vertex
		v_adj = adjc[v_index].second; // chosen vertex's adjacencies list
		if (debug_mode) printf("Current vertex: %d\n", v);

		// remove chosen vertex and its list from adjc
		adjc.erase(adjc.begin() + v_index);

		// iterate through v's adjacencies to see if it covers any new edges
		for (auto a_it = v_adj.begin(); a_it != v_adj.end(); a_it++) {
			int x = *a_it;
			if (find(Vc.begin(), Vc.end(), x) == Vc.end())
				newly_covered_edges++; // count (v, x) in as new edge if vertex x is not in Vc
		}

		if (newly_covered_edges) { // if v covers any new edges
			Vc.insert(v);
			if (debug_mode) printf("Added %d to vertex cover.\n", v);
			covered_edges += newly_covered_edges;

			if (covered_edges == total_edges) { // stop condition
				if (verify_vertex_cover(g, Vc)) {
					// finished & verified
					if (debug_mode) printf("Finished! Vertex cover size: %d\n", Vc.size());
					
					break;

				} else {
					if (debug_mode) printf("Algorithm finished but Vc didn't pass vertex cover verification :(\n");
					return INT_SET{-1};
				}
			}
			
			else if (covered_edges > total_edges) { // done fucked up
				if (debug_mode) printf("Covered edges count surpassed total edges :O\n");
				return INT_SET{-1};
			}

			else { // prepare for next iteration
				newly_covered_edges = 0;
				remaining_vertices = 0;

				for (int j = 0; j < adjc.size(); j++) {
					// iterate through all the remaining vertices and remove v from their adjacencies
					s_v_adj = adjc[j].second;
					INT_LIST::iterator pos = find(s_v_adj.begin(), s_v_adj.end(), v);
					if (pos != s_v_adj.end()) s_v_adj.erase(pos);

					adjc[j].second = s_v_adj; // necessary?
					remaining_vertices++;
				}

				sort(adjc.begin(), adjc.end(), most_values_comparator);
				// reorder the rest of adjc so that the modified adj lists are still sorted by largest to smallest
				if (debug_mode) printf("Adjacencies list reordered.\n");

			}
		}

		// else if v wouldn't cover any new edges: onto the next one
		if (debug_mode) printf("Vertex %d doesn't cover any new edges; continuing.\n", v);
	}

	return Vc;
}

/*
random multistart variation that tests diferent alphas and returns better result
*/
INT_SET random_multistart_most_neighbors_first(Graph &g, int iterations, int alpha, bool debug_mode) {

	if (iterations < 2) error("Number of iterations should be at least 2.");

	// vector<INT_SET> results;
	INT_SET result, best_result;
	int best_result_size = INT_MAX, result_size = 0;

	for (int i = 0; i < iterations; i++) {
		result = most_neighbors_first(g, alpha, i, false); // pass iterator as seed for the random number generator
		result_size = result.size();
		// results.push_back(result);

		if (debug_mode) printf("Iteration %d: found vertex cover of size %d.\n", i, result_size);

		if (result_size < best_result_size) {
			best_result = result;
			best_result_size = result_size; // save obtained MVC as the best one yet
		}
	}

	if (debug_mode) printf("%s - Best result: %d\n", g.get_name().c_str(), best_result_size);

	return best_result;
}