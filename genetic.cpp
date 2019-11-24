#include "genetic.h"


BINARY_LIST convert_solution(Graph &g, INT_SET solution) {
	BINARY_LIST binary(g.get_n());
	fill(binary.begin(), binary.end(), false);

	for (auto it = solution.begin(); it != solution.end(); it++) {
		binary[*it] = true;
	}

	return binary;
}

BINARY_SOLUTIONS initialize_population(Graph &g, int n, bool debug) {
	INT_SET solution;
	BINARY_SOLUTIONS binary_solutions;

	for (int i = 1; i <= n; i++) {
		solution = greedy(g, i);
		BINARY_LIST binary = convert_solution(g, solution);
		binary_solutions.push_back(binary);
		if (debug) printf("Initial population <- [%d]\n", cost(binary));
	}

	return binary_solutions;
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
	int crossover_point = 6; // rand() % (min_size-1);
	printf("Crossover point: %d\n", crossover_point);

	bool crossed = false;
	for (int i = crossover_point; i < min_size; i++) { // TODO: this isn't crossing over at the exact point
		if (A[i] != B[i]) {
			C[i] = B[i];
			D[i] = A[i];
			crossed = true;
		} else {
			if (crossed) break;
			else continue;
		}
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

	return (float) n / (float)(1+vc);
}

BINARY_LIST roulette_wheel_selection(BINARY_SOLUTIONS population) {
	int n = population.begin()->size();
	float fitness_sum = 0.0, random_point = -1.0, partial = 0.0;
	
	for (auto it = population.begin(); it != population.end(); it++) {
		fitness_sum += fitness(*it);
	}

	random_point = randf(0.0, fitness_sum);

	for (auto it = population.begin(); it != population.end(); it++) {
		partial += fitness(*it);

		if (partial > random_point) {
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

bool highest_fitness_comparator(const BINARY_LIST A, const BINARY_LIST B) {
	return fitness(A) > fitness(B);
}

void describe_solutions(BINARY_SOLUTIONS list) {
	printf("{ ");
	for (int i = 0; i < list.size(); i++) {
		BINARY_LIST s = list[i];
		printf("[%d f=%.3f] ", cost(s), fitness(s));
	}
	printf("}\n");
}

BINARY_LIST genetic_algorithm(Graph &g, int population_size, float crossover_fraction, float mutation_probability, int convergence, bool debug) {
	if (population_size <= 0) error("Population size must be larger than 0.");

	BINARY_SOLUTIONS population = initialize_population(g, population_size, debug);
	BINARY_LIST parent1, parent2;
	BINARY_SOLUTIONS offspring = {};
	float min_fitness = INT_MAX, max_fitness = 0;
	int converging_iterations = 0;

	for (auto it = population.begin(); it != population.end(); it++) {
		float f = fitness(*it);
		if (f < min_fitness) min_fitness = f;
		if (f > max_fitness) max_fitness = f;
	}

	if (debug) printf("Initial population: %d solutions, fitnesses: %.3f - %.3f.\n", population_size, min_fitness, max_fitness);

	while (true) { // stop criteria ?

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
			parent2 = roulette_wheel_selection(population);
			printf("Selected for crossover: [%d f=%.3f] [%d f=%.3f]\n", cost(parent1), fitness(parent1), cost(parent2), fitness(parent2));

			BINARY_SOLUTIONS local_offspring = single_point_crossover(parent1, parent2);
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

		/* new population: keep only the most fit (by population size) */
		BINARY_SOLUTIONS new_population = BINARY_SOLUTIONS(population.begin(), population.begin()+population_size);
		population = new_population;
		if (debug) printf("Added offspring and reduced population again to %d fittest solutions.\n", population.size());
		offspring = {}; // reset for next iteration

		/*
		check for convergence
		if max and min fitness in population haven't changed: increase converging iterations count 
		*/
		float max_f = fitness(population.front());
		float min_f = fitness(population.back());

		if (max_f == max_fitness && min_f == min_fitness) {
			converging_iterations++;

			if (converging_iterations >= convergence) { // the end
				if (debug) printf("Finishing after %d converging iterations.\n", converging_iterations);
				return population.front(); // best solution
			} else {
				// update stats
				max_fitness = max_f;
				min_fitness = min_f;
				if (debug) printf("Updated fitness range: %.3f - %.3f\n", min_fitness, max_fitness);
			}
		}
	}

	return BINARY_LIST_NULL;

}