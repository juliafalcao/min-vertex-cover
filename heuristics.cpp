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
INT_SET greedy(Graph &g, int seed, bool debug) {
	INT_SET Vc;
	INT_PAIR current_edge;
	int degree_a = 0, degree_b = 0, chosen = -1, a = -1, b = -1;
	
	INT_PAIR_LIST edges = g.get_edges_copy();
	if (seed) { // shuffles array if seed is not 0
		srand(seed);
		random_shuffle(edges.begin(), edges.end(), rng);
		if (debug) printf("Edges shuffled. First: (%d, %d)\n", edges[0].first, edges[0].second);
	}

	for (auto it = edges.begin(); it != edges.end(); it++) {
		a = it->first;
		b = it->second;

		if ((find(Vc.begin(), Vc.end(), a) == Vc.end()) && (find(Vc.begin(), Vc.end(), b) == Vc.end())) { // edge is yet uncovered
			degree_a = g.degree(a);
			degree_b = g.degree(b);

			if (degree_a > degree_b) {
				Vc.insert(a);
				if (debug) printf("Added %d to vertex cover.\n", a);
			} else {
				Vc.insert(b);
				if (debug) printf("Added %d to vertex cover.\n", b);
			}
		}
	}

	print(Vc);
	return Vc;
}

/*
random multistart variation that runs simple greedy multiple times and returns the best solution found
*/
INT_SET rm_greedy(Graph &g, int iterations, bool debug) {
	if (iterations < 2) error("Number of iterations should be at least 2.");

	INT_SET result, best_result;
	int best_result_size = INT_MAX, result_size = 0;

	for (int i = 0; i < iterations; i++) {
		result = greedy(g, i+1); // pass iterator++ as seed for the random number generator
		result_size = result.size();

		if (debug) printf("Iteration %d: found vertex cover of size %d.\n", i, result_size);

		if (result_size < best_result_size) {
			best_result = result;
			best_result_size = result_size; // save obtained MVC as the best one yet
		}
	}

	if (debug) printf("%s - Best result: %d\n", g.get_name().c_str(), best_result_size);

	return best_result;
}

/*
multistart alternative function that returns all solutions found
*/
vector<INT_SET> rm_greedy_all(Graph &g, int iterations, bool debug) {
	if (iterations < 2) error("Number of iterations should be at least 2.");
	
	INT_SET result;
	vector<INT_SET> results = {};

	for (int i = 0; i < iterations; i++) {
		result = greedy(g, /*i+1*/ time(NULL)); // pass iterator++ as seed for the random number generator
		results.push_back(result);

		if (debug) printf("Iteration %d: found vertex cover of size %d.\n", i, result.size());
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
INT_SET first_improving(Graph &g, INT_SET solution, int max_it, int seed, bool debug) {
	srand(seed);

	for (int i = 0; i < max_it; i++) {
		int v1 = rand() % solution.size();
		int v2 = rand() % solution.size();
		if (v2 == v1) {
			v2 = rand() % solution.size();
		}

		if (debug) printf("Selected vertices to remove: %d and %d\n", v1, v2);

		INT_PAIR_LIST uncovered = loss(g, solution, {v1, v2});

		if (debug) {
			printf("Uncovered edges: ");
			print(uncovered);
		}

		int loss = uncovered.size();
		if (debug) printf("Total loss: %d\n", loss);

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

		if (debug) {
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

			if (debug) printf("Found common vertex: %d\n", common);

			// verify vertex cover at each turn
			if (verify_vertex_cover(g, solution)) {
				return solution;
			} else {
				printf("Solution found didn't pass vertex cover validation. :(\n");
				return INT_SET_NULL;
			}
		} else {
			if (debug) printf("Iteration %d couldn't find common vertex.\n", i);
		}
	}

	if (debug) printf("Couldn't find common vertex after %d iterations. Returning INT_SET_NULL.\n", max_it);
	return INT_SET_NULL; // reached max_it and couldn't find improving solution
}


/*
local search algorithm
g: graph
max_it: max search iterations
max_it_1st: max iterations of first-improving heuristic at each search iteration
initial: optional initial solution for the search
debug: whether to print debug messages
*/
INT_SET local_search(Graph &g, int max_it, int max_it_1st, INT_SET initial, bool debug) {
	if (debug) printf("%s - Starting local search\n", g.get_name().c_str());

	INT_SET current_solution = (initial == INT_SET_NULL) ? greedy(g) /* build initial solution */ : initial;

	if (debug) printf("Initial solution:: [%d]\n", current_solution.size());

	INT_SET first_improving_solution;
	int it = 0;

	while (it < max_it) {
		first_improving_solution = first_improving(g, current_solution, max_it_1st, time(NULL), debug);

		// stop when no longer able to improve solution
		if (first_improving_solution == INT_SET_NULL) {
			if (debug) printf("[Search iteration %d] Couldn't find improving solution after %d iterations. Finishing local search.\nFinal solution: %d vertices.\n", it, max_it_1st, current_solution.size());

			if (!verify_vertex_cover(g, current_solution)) {
				printf("Final solution doesn't pass vertex cover verification.\n");
				return INT_SET_NULL;
			}

			return current_solution;
		}

		current_solution = first_improving_solution;
		it++;
	}

	return current_solution;
}

/*
random multistart version of local search, using the solutions from rm-greedy and applying local search to them, finally returning only the best result found
*/
INT_SET rm_local_search(Graph &g, string type, int it_rm, int max_it, int max_it_1st, bool debug) {
	vector<INT_SET> initial_solutions = rm_greedy_all(g, it_rm, debug); // get all rm-greedy solutions

	INT_SET solution = {}, best_ls_solution = {};
	int best_ls_solution_size = INT_MAX;
	int solution_size = 0, it_count = 0;

	for (auto it = initial_solutions.begin(); it != initial_solutions.end(); it++) {
		if (debug) printf("Applying local search to initial solution %d of size %d.\n", it_count, it->size());

		solution = local_search(g, max_it, max_it_1st, *it); // run local search using rm solution as initial
		solution_size = solution.size();

		if (solution_size <= best_ls_solution_size) {
			best_ls_solution = solution;
			best_ls_solution_size = solution_size;
		}

		it_count++;
	}

	return best_ls_solution;
}

/*
semi-greedy
uses restricted candidate list
[!] MAY RETURN INCOMPLETE VERTEX COVER (NOT A VALID SOLUTION)
*/
INT_SET semi_greedy(Graph &g, int seed, float alpha, bool debug) {
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
			// min_accepted_degree = min_degree + (int) floor(alpha*(max_degree-min_;degree)); // (!)
			min_accepted_degree = max_degree - (int) floor(alpha*(max_degree-min_degree));

			if (debug) printf("Max degree = %d, min degree = %d, min accepted = %d\n", max_degree, min_degree, min_accepted_degree);

			for (auto v_it = vertices.begin(); v_it != vertices.end(); v_it++) {
				if (g.degree(*v_it) < min_accepted_degree) break;
				RCL.push_back(*v_it);
			}
		}

		if (RCL.size() == 0) break;

		/* select random vertex from RCL and add it to cover */
		v = RCL[rand() % RCL.size()];
		if (debug) printf("Randomly chosen vertex from RCL: %d\n", v);
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
		/*if (debug)*/ printf("Shuffled edges successfully! First edge is: (%d, %d)\n", edges[0].first, edges[0].second);
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

INT_SET grasp(Graph &g, float alpha, int max_time_ms, int max_iterations, int target, bool debug) {
	INT_SET solution = {}, best_solution = {};
	int solution_size = 0, best_solution_size = INT_MAX, iterations = 0;
	int total_elapsed_time = 0, dt = 0;
	TIMESTAMP t0 = time();

	/* main loop of GRASP procedure */
	while ((iterations < max_iterations) && (total_elapsed_time < max_time_ms)) { // stopping conditions
		printf("GRASP iteration %d.\n", iterations);
		/*
		phase 1: construction (semi-greedy)
		*/
		t0 = time();
		solution = semi_greedy(g, time(NULL), alpha, false);
		dt = elapsed_time(t0); // semi-greedy runtime

		if (!verify_vertex_cover(g, solution)) {
			if (debug) printf("Semi-greedy solution (INVALID): [%d].\n", solution.size());
			t0 = time();
			solution = repair(g, solution, iterations+1);
			dt += elapsed_time(t0); // repair runtime
			if (debug) printf("Repaired solution: [%d].\n", solution.size());
		} else {
			if (debug) printf("Semi-greedy solution: [%d].\n", solution.size());
		}

		/*
		phase 2: local search
		*/
		int max_it = 100, max_it_1st = 300;
		t0 = time();
		solution = local_search(g, max_it, max_it_1st, solution);
		dt += elapsed_time(t0); // local search runtime
		solution_size = solution.size();
		if (debug) printf("Local search solution [%d].\n", solution_size);
		
		/*
		conclusion: replace current best solution if the one found is better
		*/
		if (solution_size < best_solution_size) {
			best_solution = solution;
			best_solution_size = solution_size;
		}

		/* target verification */
		if (target != -1 && best_solution_size <= target) {
			if (debug) printf("GRASP reached target value.\n");
			break;
		}

		total_elapsed_time += dt;
		iterations++;
	}

	if (debug) printf("Stopped GRASP at %d elapsed milliseconds & %d iterations.\n", total_elapsed_time, iterations);

	if (verify_vertex_cover(g, solution)) {
		return best_solution;
	} else {
		printf("GRASP returned invalid solution. D:\n");
		return INT_SET{-1};
	}
}

SOLUTION_SET restricted_neighborhood(Graph &g, INT_SET initial, INT_SET guiding) {
	SOLUTION_SET viable_solutions = {}; // set to avoid identical solutions

	if (initial == guiding) return viable_solutions; // neighborhood is empty if the solutions are the same
	else if (initial.size() < guiding.size()) {
		printf("Initial solution shouldn't be better than guiding solution.\n");
		return viable_solutions;
	}

	INT_SET possible_solution = {};

	INT_SET diff_to_remove = subtraction(initial, guiding); // elements in Si that are not in Sg
	INT_SET diff_to_include = subtraction(guiding, initial); // elements in Sg that are not in Si

	for (auto r_it = diff_to_remove.begin(); r_it != diff_to_remove.end(); r_it++) {
		possible_solution = copy_int_set(initial);
		possible_solution.erase(find(possible_solution.begin(), possible_solution.end(), *r_it));

		if (verify_vertex_cover(g, possible_solution)) viable_solutions.insert(possible_solution);

		for (auto i_it = diff_to_include.begin(); i_it != diff_to_include.end(); i_it++) {
			possible_solution.insert(*i_it);
			if (verify_vertex_cover(g, possible_solution)) viable_solutions.insert(possible_solution);
		}
	}

	return viable_solutions;
}

/*
Compute the similarity between two solutions
*/
float similarity(Graph &g, INT_SET A, INT_SET B) {
	if (A == B) return 1;

	INT_SET difference = symmetric_difference(A, B);

	float difference_cost = difference.size() / (float) g.get_n();
	return 1.0 - difference_cost;
}

/*
Given a solution set, returns the best solution considering the following criteria
1. Smallest size
2. Most similar to given solution closer_to
*/
INT_SET best_solution(Graph &g, SOLUTION_SET solutions, INT_SET closer_to, bool &tie) {
	if (solutions.size() == 1) return *solutions.begin();

	int min_size = INT_MAX, solution_size = 0;
	float max_similarity = 0.0, solution_similarity = 0.0;
	INT_SET best_solution = {}, solution = {};

	for (auto it = solutions.begin(); it != solutions.end(); it++) {
		solution = *it;
		solution_size = solution.size();
		solution_similarity = similarity(g, solution, closer_to);

		if (solution_size < min_size) {
			best_solution = solution;
			min_size = solution_size;
			max_similarity = solution_similarity;	
		} else if (solution_size == min_size) {
			if (solution_similarity > max_similarity) {
				best_solution = solution;
				min_size = solution_size;
				max_similarity = solution_similarity;
			} else if (solution_similarity == max_similarity) tie = true;
		}
	}

	return best_solution;
}

/*
Forward path relinking procedure
*/
INT_SET forward_path_relinking(Graph &g, INT_SET initial, INT_SET guiding, bool debug) {
	if (initial.size() < guiding.size()) {
		if (debug) printf("FPR called for initial solution better than guiding solution.\n");
		return initial;
	}

	printf("FPR: relinking initial [%d] and guiding [%d].\n", initial.size(), guiding.size());

	INT_SET general_best = INT_SET_NULL, neighborhood_best = INT_SET_NULL;
	int min_size = INT_MAX;
	int size_before = 0, improvement = 0; // logging
	float max_similarity = 0;
	SOLUTION_SET to_compare = {};
	int it = 0; // logging
	bool tie = false, tr;

	SOLUTION_SET rn = restricted_neighborhood(g, initial, guiding);

	while (rn.size() > 0) {
		printf("FPR ITERATION %d: RN has %d solutions.\n", it, rn.size());

		neighborhood_best = best_solution(g, rn, guiding, tr); // best & closest to guiding solution
		if (debug) printf("Neighborhood best solution: [%d].\n", neighborhood_best.size());

		if (neighborhood_best.size() > initial.size()) {
			if (debug) printf("Neighborhood best [%d] is worse than initial [%d].\n", neighborhood_best.size(), initial.size());
			return general_best != INT_SET_NULL ? general_best : initial;
		}
		
		if (neighborhood_best == general_best) { // can't keep going if the solutions are equal
			if (debug) printf("Neighborhood best and general best are the same.\n"); // TODO: verify if this happens
			break;
		}

		/* neighborhood_best vs. general_best */
		if (general_best == INT_SET_NULL) {
			general_best = neighborhood_best; // first general best
		} else {
			to_compare.insert(neighborhood_best);
			to_compare.insert(general_best);
			general_best = best_solution(g, to_compare, guiding, tie);
			if (tie) {
				printf("Neighborhood best and general best are equal in size [%d] and similarity. Finishing FPR.\n", general_best.size());
				return general_best;
			}

			to_compare.clear();
		}

		/* general best might not be a local optimum: run local search */
		size_before = general_best.size();
		general_best = local_search(g, 100, 300, general_best);
		improvement = size_before - general_best.size();
		if (debug) {
			if (!improvement) printf("Local search solution: [%d] (general best not improved).\n", general_best.size());
			else printf("Local search solution: [%d] (general best improved by %d).\n", general_best.size(), improvement);
		}

		/* update general best info */
		min_size = general_best.size();
		max_similarity = similarity(g, general_best, guiding);

		if (min_size <= guiding.size()) {
			printf("Finishing FPR: General best [%d] equal to or better than guiding [%d].\n", min_size, guiding.size());
			break;
		} else {
			// generate new restricted neighborhood to keep going
			rn = restricted_neighborhood(g, general_best, guiding);
			it++;
		}
	}

	if (debug && rn.size() == 0) printf("Finishing FPR: RN is empty.\n");

	return general_best == INT_SET_NULL ? initial : general_best;
}

/*
GRASP with path relinking
*/
SOLUTION_SET update_elite_set(INT_SET new_solution, SOLUTION_SET elite_set, int max_size, int delta_threshold, bool debug) {
	if (new_solution == INT_SET_NULL) {
		printf("[!] Update called on null solution.");
		return elite_set;
	}
	
	int current_size = elite_set.size();

	if (current_size < max_size) { // hasn't reached max_size: decide whether to include solution

		if (current_size == 0) {
			elite_set.insert(new_solution);
			if (debug) printf("Solution [%d] added to elite set.\n", new_solution.size());
			return elite_set;
		} else {
			int delta = -1, min_delta = INT_MAX;
	
			for (auto e_it = elite_set.begin(); e_it != elite_set.end(); e_it++) {
				delta = symmetric_difference(new_solution, *e_it).size();
				if (delta < min_delta) min_delta = delta;
			}

			if (min_delta > delta_threshold) { // only include if min_delta is higher than the threshold
				elite_set.insert(new_solution);
				return elite_set;
			} else {
				if (debug) printf("Elite set not full but solution [%] wasn't included.\n", new_solution.size());
			}
		}

	} else { // elite set is in its max size: decide between keeping it as it is or choosing solution to replace with new_solution
		int largest_size = 0, size = -1;
		int delta = -1, min_delta = INT_MAX;

		for (auto e_it = elite_set.begin(); e_it != elite_set.end(); e_it++) {
			/* max size */
			size = e_it->size();
			if (size > largest_size) largest_size = size;

			/* min delta */
			delta = symmetric_difference(new_solution, *e_it).size();
			if (delta < min_delta) {
				min_delta = delta;
			}
		}

		if ((new_solution.size() < largest_size) && (min_delta > delta_threshold)) { // solution should be included: choose which to remove
			auto rm_it = elite_set.end();
			min_delta = INT_MAX, delta = -1;

			for (auto e_it = elite_set.begin(); e_it != elite_set.end(); e_it++) {
				if (e_it->size() < new_solution.size()) continue;

				delta = symmetric_difference(new_solution, *e_it).size();

				if (delta < min_delta) {
					min_delta = delta;
					rm_it = e_it;
				}
			}
			if (rm_it != elite_set.end()) {
				if (debug) printf("Elite set is full -> removing [%d] to include [%d].\n", rm_it->size(), new_solution.size());
				elite_set.erase(rm_it);
				elite_set.insert(new_solution);
			} else {
				printf("YIKES");
			}
			
		}
	}

	return elite_set;
}

INT_SET grasp_pr(Graph &g, float alpha, int max_time_ms, int max_iterations, int max_elite, bool debug) {
	INT_SET solution = {}, best_solution = {}, random_elite = {};
	int solution_size = 0, best_solution_size = INT_MAX, iterations = 0, total_elapsed_time = 0, dt = 0;
	TIMESTAMP t0 = time();
	SOLUTION_SET elite_set = {};
	srand(time(NULL));

	/* main loop of GRASP procedure */
	while ((iterations < max_iterations) && (total_elapsed_time < max_time_ms)) { // stopping conditions
		printf("GRASP-PR ITERATION %d.\n", iterations);
		/*
		phase 1: construction (semi-greedy)
		*/
		t0 = time();
		solution = semi_greedy(g, time(NULL), alpha);
		dt = elapsed_time(t0); // semi-greedy runtime

		if (!verify_vertex_cover(g, solution)) {
			if (debug) printf("Semi-greedy solution (INVALID): [%d].\n", solution.size());
			t0 = time();
			solution = repair(g, solution, iterations+1);
			dt += elapsed_time(t0); // repair runtime
			if (debug) printf("Repaired solution: [%d].\n", solution.size());
		} else {
			if (debug) printf("Semi-greedy solution: [%d].\n", solution.size());
		}

		/*
		phase 2: local search
		*/
		int max_it = 100, max_it_1st = 300;
		if (debug) printf("Running local search...\n");
		t0 = time();
		solution = local_search(g, max_it, max_it_1st, solution);
		dt += elapsed_time(t0); // local search runtime
		solution_size = solution.size();
		if (debug) printf("Local search solution: [%d].\n", solution_size);

		/*
		phase 3: path relinking between current solution and a random solution from elite set
		*/
		t0 = time();
		if (elite_set.size() > 0) {
			/* choose random solution from elite set */
			if (elite_set.size() == 1) random_elite = *elite_set.begin();
			else {
				auto it = elite_set.begin();
				advance(it, rand() % elite_set.size());
				random_elite = *it;
			}

			if (debug) printf("Random elite set solution to relink: [%d].\n", random_elite.size());

			solution = forward_path_relinking(g, solution, random_elite, true);
			solution_size = solution.size();
			if (debug) printf("FPR solution: [%d].\n", solution_size);
		}

		elite_set = update_elite_set(solution, elite_set, max_elite, 0, debug);
		dt += elapsed_time(t0); // path-relinking runtime
		
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

	if (debug) printf("Stopped GRASP-PR at %d elapsed milliseconds & %d iterations.\n", total_elapsed_time, iterations);

	if (verify_vertex_cover(g, solution)) {
		return best_solution;
	} else {
		printf("GRASP returned invalid solution!\n");
		return INT_SET_NULL;
	}
}