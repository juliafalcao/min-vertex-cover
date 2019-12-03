#include "genetic.h"


BINARY_LIST convert_solution(Graph &g, INT_SET solution) {
	BINARY_LIST binary(g.get_n());
	fill(binary.begin(), binary.end(), 0);
	int v;

	for (auto it = solution.begin(); it != solution.end(); it++) {
		v = *it;
		binary[v] = 1;
	}

	return binary;
}

BINARY_LIST binary_greedy(Graph &g, int seed, bool debug) {
	BINARY_LIST S(g.get_n());
	fill(S.begin(), S.end(), 0);

	int degree_a = 0, degree_b = 0, a = -1, b = -1;
	
	INT_PAIR_LIST edges = g.edges; // won't be altered

	if (seed) { // shuffles array if seed is not 0
		srand(seed);
		random_shuffle(edges.begin(), edges.end(), rng);
		if (debug) printf("Edges shuffled. First: (%d, %d)\n", edges[0].first, edges[0].second);
	}

	for (auto it = edges.begin(); it != edges.end(); it++) {
		a = it->first, b = it->second;

		if (!S[a] && !S[b]) {
			// none of the edge vertices are in vertex cover

			if (g.degree(a) > g.degree(b)) {
				S[a] = 1;
				if (debug) printf("Included %d in vertex cover.\n", a);
			} else {
				S[b] = 1;
				if (debug) printf("Included %d in vertex cover.\n", b);
			}
		}
	}

	return S;
}

SOLUTIONS_LIST initialize_population(Graph &g, int n, bool debug) {
	if (debug) printf("Initializing population...\n");
	SOLUTIONS_LIST solutions = {};
	BINARY_LIST solution;

	for (int i = 1; i <= n; i++) {
		if (debug) printf("Running binary-greedy construction...\n");
		solution = binary_greedy(g, i, false);
		solutions.push_back(solution);
		if (debug) printf("Initial population <- [%d]\n", cost(solution));
	}

	return solutions;
}

int cost(BINARY_LIST solution) {
	return count(solution.begin(), solution.end(), 1);
}

SOLUTIONS_LIST single_point_crossover(BINARY_LIST A, BINARY_LIST B, bool debug) {
	if (A == B) {
		printf("Chromosomes to crossover should be different.\n");
		return SOLUTIONS_LIST_NULL;
	} else if (A.size() != B.size()) error("Chromosomes should be the same size.");

	SOLUTIONS_LIST offspring = {};
	BINARY_LIST C = copy(A);
	BINARY_LIST D = copy(B);
	int size = A.size();
	
	INT_LIST candidate_points = {};

	for (int i = 0; i < size; i++) {
		if (A[i] != B[i]) candidate_points.push_back(i);
	}

	if (!candidate_points.size()) error("No candidate points for single-point crossover.");

	if (debug) printf("Candidate crossover points: %d candidates\n", candidate_points.size());
	int crossover_point = candidate_points[rand() % candidate_points.size()];
	if (debug) printf("Chosen crossover point: %d\n", crossover_point);

	for (int i = crossover_point; i < size; i++) {
		C[i] = B[i];
		D[i] = A[i];
	}

	offspring.push_back(C);
	offspring.push_back(D);

	return offspring;
}

BINARY_LIST mutation(BINARY_LIST solution, float probability, bool &m) {
	if (probability <= 0.0 || probability >= 1.0) {
		printf("Probability of mutate should be between 0.0 and 1.0.\n");
		return BINARY_LIST{-1};
	}
	
	bool mutate = randf(0.0, 1.0) < probability;
	m = mutate; // logging
	if (mutate) {
		int index = rand() % solution.size();
		printf("Solution selected for mutate -> flipping bit at index %d.\n", index);
		solution[index] = !solution[index]; // flip bit
	}

	return solution;
}

float fitness(BINARY_LIST solution) {
	int n = solution.size(); // graph size
	int vc = cost(solution); // solution "size" (cost)

	if (vc <= 1) {
		error("Solution too small.");
	}

	return (float) n / (float)(1+vc);
}

BINARY_LIST roulette_wheel_selection(SOLUTIONS_LIST population) {
	if (population.size() <= 1) error("Population too small for selection.");
	int n = population.begin()->size();
	float fitness_sum = 0.0, random_point = 0.0, partial = 0.0;
	
	for (auto it = population.begin(); it != population.end(); it++) {
		fitness_sum += fitness(*it);
	}

	random_point = randf(0.0, fitness_sum);
	printf("[Selection] random point = %.3f, fitness sum = %.3f\n", random_point, fitness_sum);

	for (auto it = population.begin(); it != population.end(); it++) {
		partial += fitness(*it);

		if (partial >= random_point) {
			return *it;
		}
	}

	return BINARY_LIST_NULL;
}

bool verify_solution(Graph &g, BINARY_LIST solution) {
	int a = -1, b = -1;
	for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
		a = it->first, b = it->second;

		if (!solution[a] && !solution[b]) { // found uncovered edge
			return false;
		}
	}

	return true;
}

BINARY_LIST verify_and_repair(Graph &g, BINARY_LIST solution, bool &r) {
	int a = -1, b = -1;
	r = false; // to record if solution had to be repaired or not
	for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
		a = it->first, b = it->second;

		if (!solution[a] && !solution[b]) { // edge is uncovered
			r = true;
			if (g.degree(a) >= g.degree(b)) {
				solution[a] = 1; // add vertex to solution
			} else {
				solution[b] = 1;
			}
		}
	}

	return solution;
}

bool highest_fitness_comparator(const BINARY_LIST &A, const BINARY_LIST &B) {
	float fA = fitness(A), fB = fitness(B);
	return fA > fB;
}

void describe_solutions(SOLUTIONS_LIST list) {
	printf("{ ");
	for (int i = 0; i < list.size(); i++) {
		BINARY_LIST s = list[i];
		printf("[%d f=%.3f] ", cost(s), fitness(s));
	}
	printf("}\n");
}

/*
generate amount of new different solutions randomly based on the given one
*/
SOLUTIONS_LIST diversify(Graph &g, BINARY_LIST solution, int amount, bool debug) {
	int n = solution.size();
	set<BINARY_LIST> solutions_set = {};
	if (debug) printf("Diversify will generate %d new solutions from [%d].\n", amount, cost(solution));

	while (solutions_set.size() != amount) {
		int changes = rand() % (int) ceil(0.15*n); // some amount between 0 and 10% of graph size

		for (int _ = 0; _ < changes; _++) {
			int index = rand() % n;
			solution[index] = !solution[index]; // flip bit at random position
		}

		if (debug) printf("New diverse [%d] generated (%d bits flipped).\n", cost(solution), changes);

		bool tr;
		solution = verify_and_repair(g, solution, tr); // repair modified solution if needed
		if (debug) printf("Solution [%d] repaired.\n", cost(solution));

		solutions_set.insert(solution);
	}

	// convert to vector instead of set
	SOLUTIONS_LIST solutions = SOLUTIONS_LIST(solutions_set.begin(), solutions_set.end());

	return solutions;
}

BINARY_LIST genetic_algorithm(Graph &g, int np, float crossover, float mutation_probability, int stability, bool debug) {
	if (np <= 2) error("Population size must be larger than 2 individuals.");

	srand(time(0)); // seed all generators
	
	SOLUTIONS_LIST population = initialize_population(g, np, debug);	

	BINARY_LIST parent1, parent2;
	SOLUTIONS_LIST offspring = {};
	float min_fitness = INT_MAX, max_fitness = 0;
	int stable_iterations = 0, gen = 0;

	for (auto it = population.begin(); it != population.end(); it++) {
		float f = fitness(*it);
		if (f > max_fitness) max_fitness = f; // initialize max fitness
		if (f < min_fitness) min_fitness = f; // initialize min fitness (logging)
	}

	if (debug) printf("Initial population: %d solutions, fitnesses: %.3f - %.3f.\n", np, min_fitness, max_fitness);

	while (true) { // stop criteria ?
		printf("GENETIC ALGORITHM GEN %d.\n", gen);

		/*
		remove duplicate solutions from population
		convert to set and back to list
		*/
		BINARY_LIST_SET unique_population = BINARY_LIST_SET(population.begin(), population.end());
		if (debug) printf("Population (%d) reduced to %d unique solution(s).\n", population.size(), unique_population.size());
		population = SOLUTIONS_LIST(unique_population.begin(), unique_population.end());

		/*
		crossover
		*/
		int crossover_count = ceil(crossover*np);
		if (debug) printf("Crossover count: %d\n", crossover_count);

		for (int z = 0; z < crossover_count; z++) {
			parent1 = roulette_wheel_selection(population);
			do {
				parent2 = roulette_wheel_selection(population);
			} while (parent2 == parent1); // to guarantee parent1 != parent2

			printf("Selected for crossover: [%d f=%.3f] [%d f=%.3f]\n", cost(parent1), fitness(parent1), cost(parent2), fitness(parent2));

			SOLUTIONS_LIST local_offspring = single_point_crossover(parent1, parent2);
			if (local_offspring == SOLUTIONS_LIST_NULL) error("Local offspring is null.");
			printf("Generated offspring: [%d f=%.3f] [%d f=%.3f]\n", cost(local_offspring.front()), fitness(local_offspring.front()), cost(local_offspring.back()), fitness(local_offspring.back()));
			offspring.insert(offspring.end(), local_offspring.begin(), local_offspring.end());
		}

		printf("Offspring: %d solutions\n", offspring.size());

		/*
		mutate offspring
		*/
		int mutated = 0;
		bool m;
		for (auto it = offspring.begin(); it != offspring.end(); it++) {
			*it = mutation(*it, mutation_probability, m);
			if (m) mutated++;
		}

		if (debug) printf("Mutated %d/%d offspring solutions.\n", mutated, offspring.size());

		/*
		validate offspring & repair invalid solutions
		*/
		int repaired = 0;
		bool r;
		for (auto it = offspring.begin(); it != offspring.end(); it++) {
			*it = verify_and_repair(g, *it, r); // TODO: test
			if (r) repaired++;
		}

		if (debug) printf("Repaired %d/%d invalid offspring solutions.\n", repaired, offspring.size());

		/*
		eval fitness: add offspring to population and sort by fitness
		*/
		population.insert(population.end(), offspring.begin(), offspring.end());
		sort(population.begin(), population.end(), highest_fitness_comparator);

		/* new population: keep only the most fit (by population size) */
		SOLUTIONS_LIST new_population = SOLUTIONS_LIST(population.begin(), population.begin()+np);
		population = new_population;
		if (debug) printf("Added offspring and reduced population again to %d fittest solutions.\n", population.size());
		offspring = {}; // reset for next iteration

		/*
		check for stabilization
		*/
		float max_f = fitness(population.front());
		printf("Highest fitness = %.3f\n", max_f);

		if (max_f > max_fitness) { // still improving
			max_fitness = max_f;
			stable_iterations = 0;
			printf("Updated max fitness: %.3f\n", max_fitness);
		}
		if (max_f == max_fitness) { // stabilizing
			stable_iterations++;
			printf("Stabilizing %d/%d.\n", stable_iterations, stability);

			if (stable_iterations >= stability) { // the end
				BINARY_LIST best = population.front();
				if (verify_solution(g, best)) {
					if (debug) printf("Finishing after %d stable iterations.\nBest solution: [%d].\n", stable_iterations, cost(best));
					return population.front(); // best solution
				} else {
					printf("Best solution at end of genetic algorithm is invalid.\n");
					return BINARY_LIST_NULL;
				}
				
			}
		} else { // wrong
			error("ERROR: Best fitness shouldn't go down.");
		}

		gen++;
	}

	return BINARY_LIST_NULL;
}