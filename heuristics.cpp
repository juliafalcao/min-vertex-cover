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

// aux
int rng(int i) {
	return rand() % i;
}

/*
simple greedy constructive heuristic
iterates through all edges and, if the edge is uncovered, adds its vertex with the highest degree to vertex cover
*/
INT_SET greedy(Graph &g, int seed, bool debug_mode) {
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
random multistart variation that runs simple greedy multiple times and returns the best solution found
*/
INT_SET rm_greedy(Graph &g, int iterations, bool debug_mode) {
	if (iterations < 2) error("Number of iterations should be at least 2.");

	INT_SET result, best_result;
	int best_result_size = INT_MAX, result_size = 0;

	for (int i = 0; i < iterations; i++) {
		result = greedy(g, i+1); // pass iterator++ as seed for the random number generator
		result_size = result.size();

		if (debug_mode) printf("Iteration %d: found vertex cover of size %d.\n", i, result_size);

		if (result_size < best_result_size) {
			best_result = result;
			best_result_size = result_size; // save obtained MVC as the best one yet
		}
	}

	if (debug_mode) printf("%s - Best result: %d\n", g.get_name().c_str(), best_result_size);

	return best_result;
}

/*
multistart alternative function that returns all solutions found
*/
vector<INT_SET> rm_greedy_all(Graph &g, int iterations, bool debug_mode) {
	if (iterations < 2) error("Number of iterations should be at least 2.");
	
	INT_SET result;
	vector<INT_SET> results = {};

	for (int i = 0; i < iterations; i++) {
		result = greedy(g, /*i+1*/ time(NULL)); // pass iterator++ as seed for the random number generator
		results.push_back(result);

		if (debug_mode) printf("Iteration %d: found vertex cover of size %d.\n", i, result.size());
	}


	return results;
}

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
g: graph
type: "first" for first-improving, "best" for best-improving
max_it: max search iterations
max_it_1st: max iterations of first-improving heuristic at each search iteration
&search_iterations: variable to store the number of actual search iterations run
initial_solution: optional initial solution for the search
debug_mode: whether to print debug messages
*/
INT_SET local_search(Graph &g, string type, int max_it, int max_it_1st, int &search_iterations, INT_SET initial_solution, bool debug_mode) {
	if (debug_mode) printf("%s - Starting local search\n", g.get_name().c_str());

	INT_SET current_solution = (initial_solution == INT_SET_NULL) ? greedy(g) /* build initial solution */ : initial_solution;

	if (debug_mode) printf("Size of initial solution: %d\n", current_solution.size());

	INT_SET first_improving_solution;
	int it = 0;

	while (it < max_it) {
		first_improving_solution = first_improving(g, current_solution, max_it_1st, time(NULL), debug_mode); // passes iterator as seed to guarantee each first_improving call will select different vertices

		// stop when no longer able to improve solution
		if (first_improving_solution == INT_SET_NULL) {
			if (debug_mode) printf("[Search iteration %d] Couldn't find improving solution after %d iterations. Finishing local search.\nFinal solution: %d vertices.\n", it, max_it_1st, current_solution.size());

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

	search_iterations = it;
	return current_solution;
}

/*
random multistart version of local search, using the solutions from rm-greedy and applying local search to them, finally returning only the best result found
*/
INT_SET rm_local_search(Graph &g, string type, int it_rm, int max_it, int max_it_1st, int &improvement, bool debug_mode) {
	vector<INT_SET> initial_solutions = rm_greedy_all(g, it_rm, debug_mode); // get all rm-greedy solutions

	INT_SET solution = {}, best_ls_solution = {};
	int best_ls_solution_size = INT_MAX;
	int search_iterations = 0 /*trash*/, solution_size = 0, initial_size = 0, it_count = 0;

	for (auto it = initial_solutions.begin(); it != initial_solutions.end(); it++) {
		if (debug_mode) printf("Applying local search to initial solution %d of size %d.\n", it_count, it->size());

		solution = local_search(g, "first", max_it, max_it_1st, search_iterations, *it); // run local search using rm solution as initial
		solution_size = solution.size();

		if (solution_size <= best_ls_solution_size) {
			best_ls_solution = solution;
			best_ls_solution_size = solution_size;
			initial_size = it->size(); // store size of initial_solution used for this best ls solution
		}

		it_count++;
	}

	if (initial_size == best_ls_solution_size) {
		if (debug_mode) printf("LS didn't improve the solution used at all :(\n");
		improvement = 0;
	}

	else {
		improvement = initial_size - best_ls_solution_size;
	}

	return best_ls_solution;
}

/*
semi-greedy
uses restricted candidate list
[!] MAY RETURN INCOMPLETE VERTEX COVER (NOT A VALID SOLUTION)
*/
INT_SET semi_greedy(Graph &g, int seed, float alpha, bool debug_mode) {
	if ((alpha < 0.0) || (alpha > 1.0)) {
		error("Invalid alpha. (0 <= alpha <= 1)");
	}

	INT_SET Vc = {};

	/* get vertex list and sort it by highest degree first */
	INT_LIST vertices = g.get_vertex_list();
	vertices = g.sort_vertices_by_higher_degree(vertices);
	int min_degree = g.min_degree(), max_degree = g.max_degree();

	if (g.degree(vertices[0]) != max_degree || g.degree(vertices[vertices.size()-1]) != min_degree) { // validate sorting
		error("Vertices list wasn't sorted properly.");
	}

	INT_LIST RCL = {};
	int min_accepted_degree = -1, v = -1;
	if (seed) srand(seed); // seed generator once
	int max_it = g.get_m(), it = 0;

	/* main loop */
	while (it < max_it) {

		/* build RCL */
		RCL.clear();
		
		if (alpha == 1.0) {
			for (auto v_it = vertices.begin(); v_it != vertices.end(); v_it++) {
				if (find(Vc.begin(), Vc.end(), *v_it) != Vc.end()) continue;
				RCL.push_back(*v_it);
			}
		} else {
			max_degree = g.degree(vertices.front());
			min_degree = g.degree(vertices.back());
			min_accepted_degree = min_degree + (int) floor(alpha*(max_degree-min_degree));

			for (auto v_it = vertices.begin(); v_it != vertices.end(); v_it++) {
				if (g.degree(*v_it) < min_accepted_degree) break;
				// if (find(Vc.begin(), Vc.end(), *v_it) != Vc.end()) continue; // skip vertices already in cover
				RCL.push_back(*v_it);
			}
		}

		if (RCL.size() == 0) break;

		/* select random vertex from RCL and add it to cover */
		v = RCL[rand() % RCL.size()];
		Vc.insert(v);
		vertices.erase(find(vertices.begin(), vertices.end(), v));

		/* if it's a complete cover, stop running */
		if (verify_vertex_cover(g, Vc)) break;

		it++;
	}

	return Vc;
}

/*
repair incomplete vertex cover by iterating through edges and, for uncovered edges, adding the vertex of highest degree to the solution
*/
INT_SET repair(Graph &g, INT_SET incomplete_Vc, int seed) {
	INT_SET Vc = incomplete_Vc;
	int a = 0, b = 0;

	INT_PAIR_LIST edges = g.get_edges_copy();

	if (seed) {
		srand(seed);
		random_shuffle(edges.begin(), edges.end(), rng);
		/*if (debug_mode)*/ printf("Shuffled edges successfully! First edge is: (%d, %d)\n", edges[0].first, edges[0].second);
	}

	for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
		a = it->first;
		b = it->second;

		if ((find(Vc.begin(), Vc.end(), a) == Vc.end()) && (find(Vc.begin(), Vc.end(), b) == Vc.end())) {
			// edge is uncovered

			if (g.degree(a) > g.degree(b)) { // add vertex with the highest degree to incomplete Vc
				Vc.insert(a);
			} else {
				Vc.insert(b);
			}
		}
	}

	return Vc;
}

INT_SET grasp(Graph &g, float alpha, int max_time_ms, int max_iterations, bool debug_mode) {
	INT_SET solution = {}, best_solution = {};
	int solution_size = 0, best_solution_size = INT_MAX;
	int total_elapsed_time = 0, dt = 0;
	TIMESTAMP t0 = time();
	int iterations = 0;

	INT_SET previous_solution = {};

	/* main loop of GRASP procedure */
	while ((iterations < max_iterations) && (total_elapsed_time < max_time_ms)) { // stopping conditions
		printf("GRASP iteration %d.\n", iterations);
		/*
		phase 1: construction
		*/
		t0 = time();
		solution = semi_greedy(g, iterations+1, alpha, true);
		dt = elapsed_time(t0);

		if (solution == previous_solution) {
			if (debug_mode) printf("New semi-greedy 'solution' is the exact same as the previous one.\n");
		}

		previous_solution = solution;

		if (!verify_vertex_cover(g, solution)) {
			if (debug_mode) printf("Constructed solution by semi-greedy is incomplete (%d vertices); will repair.\n", solution.size());
			t0 = time();
			solution = repair(g, solution, iterations+1);
			dt += elapsed_time(t0);
			if (debug_mode) printf("Repaired solution now has %d vertices.\n", solution.size());
		} else {
			if (debug_mode) printf("Constructed solution is valid; onto local search now.\n");
		}

		/*
		phase 2: local search
		*/
		int max_it = 100, max_it_1st = 300, search_iterations = -1;
		t0 = time();
		solution = local_search(g, "first", max_it, max_it_1st, search_iterations, solution);
		dt += elapsed_time(t0);
		solution_size = solution.size();
		if (debug_mode) printf("Local search found MVC of size %d in %d search iterations.\n", solution_size, search_iterations);
		
		/*
		conclusion: replace current best solution if the one found is better
		*/
		if (solution_size < best_solution_size) {
			best_solution = solution;
			best_solution_size = solution_size;
		}

		total_elapsed_time += dt;
		iterations++;
	}

	if (debug_mode) printf("Stopped GRASP at %d elapsed milliseconds & %d iterations.\n", total_elapsed_time, iterations);

	if (verify_vertex_cover(g, solution)) {
		return best_solution;
	} else {
		printf("GRASP returned invalid solution. D:\n");
		return INT_SET{-1};
	}

}