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
					return INT_SET_NULL;
				}
			}
			
			else if (covered_edges > total_edges) { // done fucked up
				if (debug_mode) printf("Covered edges count surpassed total edges :O\n");
				return INT_SET_NULL;
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
*/

/*
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
*/

// aux
int rng(int i) {
	return rand() % i;
}

/*
simple greedy constructive heuristic
iterates through all edges and, if the edge is uncovered, adds its vertex with the highest degree to vertex cover
*/

INT_SET simple_greedy(Graph &g, int seed, bool debug_mode) {
	INT_SET Vc;
	INT_PAIR current_edge;
	int degree_a = 0, degree_b = 0, chosen = -1, a = -1, b = -1;
	
	INT_PAIR_LIST edges = g.get_edges_copy();
	if (seed) { // shuffles array if seed is not 0
		srand(seed);
		random_shuffle(edges.begin(), edges.end(), rng);
		if (debug_mode) printf("Shuffled edges successfully! First edge is: (%d, %d)\n", edges[0].first, edges[0].second);
	}

	for (auto it = edges.begin(); it != edges.end(); it++) {
		current_edge = *it;
		a = current_edge.first;
		b = current_edge.second;

		if ((find(Vc.begin(), Vc.end(), a) == Vc.end()) && (find(Vc.begin(), Vc.end(), b) == Vc.end())) { // neither a nor b is in vertex cover: edge is yet uncovered
			degree_a = g.degree(current_edge.first);
			degree_b = g.degree(current_edge.second);

			if (degree_a > degree_b) {
				chosen = current_edge.first;
			} else {
				chosen = current_edge.second;
			}

			Vc.insert(chosen); // add edge vertex with the highest degree to vertex cover
		}
	}

	if (verify_vertex_cover(g, Vc)) {
		return Vc; // the end
	} else {
		printf("Algorithm finished but Vc didn't pass vertex cover verification :O\n");
		return INT_SET_NULL;
	}
}

/*
random multistart variation that tests diferent alphas and returns better result
*/
INT_SET random_multistart_simple_greedy(Graph &g, int iterations, bool debug_mode) {
	if (iterations < 2) error("Number of iterations should be at least 2.");

	INT_SET result, best_result;
	int best_result_size = INT_MAX, result_size = 0;

	for (int i = 0; i < iterations; i++) {
		result = simple_greedy(g, i+1); // pass iterator++ as seed for the random number generator
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


int count_occurrences(INT_LIST A, int x) {
	int count = 0;

	for (auto it = A.begin(); it != A.end(); it++) {
		if (*it == x) count++;
	}

	return count;
}

/*
edges that will become uncovered by the removal of v
OBS.: fails because maybe loss(v1) is empty and loss(v2) is empty, but (v1,v2) is an edge and would be uncovered by the removal of both v1 and v2
*/
/*
INT_PAIR_LIST loss(Graph &g, INT_SET Vc, int v) {
	int a = -1, b = -1;
	INT_PAIR_LIST loss = {};

	for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
		a = it->first;
		b = it->second;

		if (a == v) {
			if (find(Vc.begin(), Vc.end(), b) == Vc.end()) {
				// if a will be removed and b is not in Vc: edge will become uncovered
				loss.push_back(make_pair(a, b));
			}
		} else if (b == v) {
			if (find(Vc.begin(), Vc.end(), a) == Vc.end()) {
				// if b will be removed and a is not in Vc: edge will become uncovered
				loss.push_back(make_pair(a, b));
			}

		}
	}

	return loss;
}
*/

/*
edges that will become uncovered by the removal of all the vertices in V from Vc
*/
INT_PAIR_LIST loss(Graph &g, INT_SET Vc, INT_SET V) {
	int a = -1, b = -1;
	INT_PAIR_LIST loss = {};
	bool removing_a = false, removing_b = false;

	for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
		a = it->first;
		b = it->second;
		removing_a = find(V.begin(), V.end(), a) != V.end();
		removing_b = find(V.begin(), V.end(), b) != V.end();

		if ((removing_a && removing_b) || // a and b are to be removed: (a,b) would be uncovered
		(removing_a && find(Vc.begin(), Vc.end(), b) == Vc.end()) || // a to be removed, b not in cover: (a,b) would be uncovered
		(removing_b && find(Vc.begin(), Vc.end(), a) == Vc.end()) // b to be removed, a not in cover: (a,b) would be uncovered
		) {
			loss.push_back(make_pair(a, b));
		}
	}

	return loss;
}


/*
returns first solution that improves the given solution, or -1 if it reaches max_it iterations
*/
INT_SET first_improving(Graph &g, INT_SET solution, int max_it, int seed, bool debug_mode) {
	srand(seed);

	for (int i = 0; i < max_it; i++) {
		int v1 = rand() % solution.size();
		int v2 = rand() % solution.size();
		if (v2 == v1) {
			v2 = rand() % solution.size();
		}

		if (debug_mode) printf("Selected vertices to remove: %d and %d\n", v1, v2);

		int a = -1, b = -1;

		INT_PAIR_LIST uncovered = loss(g, solution, {v1, v2});

		if (debug_mode) {
			printf("Uncovered edges: ");
			print_int_pair_list(uncovered);
		}

		int loss = uncovered.size();
		if (debug_mode) printf("Total loss: %d\n", loss);

		int common = -1; // vertex (most likely non existent) that covers all of the edges uncovered by the loss of v1 and v2
		INT_LIST flattened = {};

		for (auto it = uncovered.begin(); it != uncovered.end(); it++) {
			flattened.push_back(it->first);
			flattened.push_back(it->second);
		}

		for (auto it = flattened.begin(); it != flattened.end(); it++) {
			if (*it == v1 || *it == v2) continue; // common vertex can't be the 2 removed
			if (count_occurrences(flattened, *it) == loss) {
				// found a vertex that covers all lost edges!
				common = *it;
				break;
			}
		}

		if (debug_mode) {
			printf("Flattened: ");
			for (auto it = flattened.begin(); it != flattened.end(); it++) {
				printf("%d ", *it);
			}
			printf("\n");
		}

		if (common != -1) {
			// remove v1 and v2 and insert common vertex
			solution.erase(v1);
			solution.erase(v2);
			solution.insert(common);

			if (debug_mode) printf("Found common vertex: %d\n", common);

			// verify vertex cover at each turn
			if (verify_vertex_cover(g, solution)) {
				return solution;
			} else {
				printf("Solution found didn't pass vertex cover validation. :(\n");
				return INT_SET_NULL;
			}
		} else {
			if (debug_mode) printf("Iteration %d couldn't find common vertex.\n", i);
		}
	}

	if (debug_mode) printf("Couldn't find common vertex after %d iterations. Returning INT_SET_NULL.\n", max_it);
	return INT_SET_NULL; // reached max_it and couldn't find improving solution
}


/*
local search algorithm
*/
INT_SET local_search(Graph &g, int max_it_first_improving, int &search_iterations, bool debug_mode) {
	if (debug_mode) printf("%s - Starting local search\n", g.get_name().c_str());

	INT_SET current_solution = simple_greedy(g); // build initial solution
	if (debug_mode) printf("Size of initial solution: %d\n", current_solution.size());

	INT_SET first_improving_solution;
	int it = 0;

	while (true) { // ouch
		first_improving_solution = first_improving(g, current_solution, max_it_first_improving, it, debug_mode); // passes iterator as seed to guarantee each first_improving call will select different vertices

		// stop when no longer able to improve solution
		if (first_improving_solution == INT_SET_NULL) {
			if (debug_mode) printf("[Search iteration %d] Couldn't find improving solution after %d iterations. Finishing local search.\nFinal solution: %d vertices.\n", it, max_it_first_improving, current_solution.size());

			if (!verify_vertex_cover(g, current_solution)) {
				printf("YIKES! Final solution doesn't pass vertex cover verification. D:\n");
				return INT_SET_NULL;
			}

			search_iterations = it;
			return current_solution;
		}

		current_solution = first_improving_solution;
		it++;
	}
}