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

BINARY_SOLUTIONS initialize_population(Graph &g, int n, bool debug) {
	INT_SET solution;
	BINARY_SOLUTIONS solutions;
	BINARY_LIST binary;

	for (int i = 1; i <= n; i++) {
		solution = greedy(g, i);
		binary = convert_solution(g, solution);
		solutions.push_back(binary);
		if (debug) printf("Initial population <- [%d]\n", cost(binary));
	}

	return solutions;
}

int cost(BINARY_LIST solution) {
	return count(solution.begin(), solution.end(), 1);
}

BINARY_SOLUTIONS single_point_crossover(BINARY_LIST A, BINARY_LIST B) {
	if (A == B) {
		printf("Chromosomes to crossover should be different.\n");
		return BINARY_SOLUTIONS_NULL;
	}

	BINARY_SOLUTIONS offspring = {};
	BINARY_LIST C = copy(A);
	BINARY_LIST D = copy(B);
	
	int min_size = A.size() < B.size() ? A.size() : B.size();
	INT_LIST candidate_points = {};

	for (int i = 0; i < min_size; i++) {
		if (A[i] != B[i]) candidate_points.push_back(i);
	}

	printf("Candidate crossover points: ");
	print(candidate_points);

	int crossover_point = candidate_points[rand() % candidate_points.size()];
	printf("Chosen crossover point: %d\n", crossover_point);

	for (int i = crossover_point; i < min_size; i++) {
		if (A[i] == B[i]) break;

		C[i] = B[i];
		D[i] = A[i];
	}

	if (C != A) offspring.push_back(C);
	if (D != B) offspring.push_back(D);

	return offspring;
}

BINARY_LIST mutation(BINARY_LIST solution, float probability, bool &m) {
	if (probability <= 0.0 || probability >= 1.0) {
		printf("Probability of mutation should be between 0.0 and 1.0.\n");
		return BINARY_LIST{-1};
	}
	
	bool mutate = randf(0.0, 1.0) < probability;
	m = mutate; // for logging purposes
	if (mutate) {
		int index = rand() % solution.size();
		printf("Solution selected for mutation -> flipping bit at index %d.\n", index);
		solution[index] = !solution[index]; // flip bit
	}

	return solution;
}

float fitness(BINARY_LIST solution) {
	int n = solution.size(); // graph size
	int vc = cost(solution); // solution "size" (cost)

	if (vc <= 1) {
		printf("Solution too small: ");
		print(solution);
		error("");
	}

	return (float) n / (float)(1+vc);
}

BINARY_LIST roulette_wheel_selection(BINARY_SOLUTIONS population) {
	if (population.size() <= 1) error("Population too small.");
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

void describe_solutions(BINARY_SOLUTIONS list) {
	printf("{ ");
	for (int i = 0; i < list.size(); i++) {
		BINARY_LIST s = list[i];
		printf("[%d f=%.3f] ", cost(s), fitness(s));
	}
	printf("}\n");
}

BINARY_LIST genetic_algorithm(Graph &g, int population_size, float crossover_fraction, float mutation_probability, int stable_it_max, bool debug) {
	if (population_size <= 0) error("Population size must be larger than 0.");

	srand(time(0)); // seed all generators

	BINARY_SOLUTIONS population = initialize_population(g, population_size, debug);
	BINARY_LIST parent1, parent2;
	BINARY_SOLUTIONS offspring = {};
	float min_fitness = INT_MAX, max_fitness = 0;
	int stable_iterations = 0, gen = 0;

	for (auto it = population.begin(); it != population.end(); it++) {
		float f = fitness(*it);
		if (f > max_fitness) max_fitness = f; // initialize max fitness
	}

	if (debug) printf("Initial population: %d solutions, fitnesses: %.3f - %.3f.\n", population_size, min_fitness, max_fitness);

	while (true) { // stop criteria ?
		printf("GENETIC ALGORITHM GEN %d.\n", gen);

		/*
		remove duplicate solutions from population
		convert to set and back to list
		*/
		BINARY_LIST_SET unique_population = BINARY_LIST_SET(population.begin(), population.end());
		population = BINARY_SOLUTIONS(unique_population.begin(), unique_population.end());

		/*
		select parents for crossover
		*/
		int crossover_count = ceil(crossover_fraction*population_size);
		if (debug) printf("Crossover count: %d\n", crossover_count);

		for (int z = 0; z < crossover_count; z++) {
			parent1 = roulette_wheel_selection(population);
			do {
				parent2 = roulette_wheel_selection(population);
			} while (parent2 == parent1); // to guarantee parent1 != parent2

			printf("Selected for crossover: [%d f=%.3f] [%d f=%.3f]\n", cost(parent1), fitness(parent1), cost(parent2), fitness(parent2));

			BINARY_SOLUTIONS local_offspring = single_point_crossover(parent1, parent2);
			if (local_offspring == BINARY_SOLUTIONS_NULL) error("Local offspring is null.");
			printf("Generated offspring: [%d f=%.3f] [%d f=%.3f]\n", cost(local_offspring.front()), fitness(local_offspring.front()), cost(local_offspring.back()), fitness(local_offspring.back()));
			offspring.insert(offspring.end(), local_offspring.begin(), local_offspring.end());
		}

		printf("Offspring: %d solutions\n", offspring.size());

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
		offspring ready -> mutate?
		*/
		int mutated = 0;
		bool m;
		for (auto it = offspring.begin(); it != offspring.end(); it++) {
			*it = mutation(*it, mutation_probability, m);
			if (m) mutated++;
		}

		if (debug) printf("Mutated %d/%d offspring solutions.\n", mutated, offspring.size());
		// Mutated solutions might be invalid!

		/*
		eval fitness: add offspring to population and sort by fitness
		*/
		population.insert(population.end(), offspring.begin(), offspring.end());
		sort(population.begin(), population.end(), highest_fitness_comparator);
		printf("Population:\n");
		describe_solutions(population);

		/* new population: keep only the most fit (by population size) */
		BINARY_SOLUTIONS new_population = BINARY_SOLUTIONS(population.begin(), population.begin()+population_size);
		population = new_population;
		if (debug) printf("Added offspring and reduced population again to %d fittest solutions.\n", population.size());
		offspring = {}; // reset for next iteration

		/*
		check for stabilization
		if max fitness in population hasn't changed: it's stabilizing 
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
			printf("Stabilizing %d/%d.\n", stable_iterations, stable_it_max);

			if (stable_iterations >= stable_it_max) { // the end
				if (debug) printf("Finishing after %d stable iterations.\n", stable_iterations);
				return population.front(); // best solution
			}
		} else { // wrong
			error("ERROR: Best fitness shouldn't go down.");
		}

		gen++;
	}

	return BINARY_LIST_NULL;

}