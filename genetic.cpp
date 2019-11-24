#include "genetic.h"


BINARY_LIST convert_solution(Graph &g, INT_SET solution) {
	BINARY_LIST binary(g.get_n());
	fill(binary.begin(), binary.end(), false);

	for (auto it = solution.begin(); it != solution.end(); it++) {
		binary[*it] = true;
	}

	return binary;
}

BINARY_SOLUTIONS initialize_population(Graph &g, int n) {
	INT_SET solution;
	BINARY_SOLUTIONS binary_solutions;

	for (int i = 1; i <= n; i++) {
		solution = greedy(g, i);
		BINARY_LIST binary = convert_solution(g, solution);
		binary_solutions.push_back(binary);
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

	print(A);
	print(B);

	srand(time(NULL));
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

BINARY_LIST mutation(BINARY_LIST solution, float probability) {
	if (probability <= 0.0 || probability >= 1.0) {
		printf("Probability of mutation should be between 0.0 and 1.0.\n");
		return BINARY_LIST{-1};
	}

	bool mutate = (rand() % 100) < (probability*100);
	if (mutate) {
		int index = rand() % solution.size();
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

BINARY_LIST genetic_algorithm(Graph &g) {

	BINARY_SOLUTIONS population = initialize_population(g, 50);



	// while (true) { // stop criteria ?
		// /* select parents for crossover */

	// }

	return BINARY_LIST_NULL;

}