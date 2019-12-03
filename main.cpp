#include "main.h"

#define GRAPHS_PATH "instances/"

int main(int argc, char *argv[]) {
	int pop, stability;
	float crossover, mutation;
	string instance;
	
	if (argc == 1) {
		error("No instance name passed.\n");
	} else if (argc == 2) { // only program name and instance name: use default genetic algorithm params
		instance = argv[1];
		pop = 60;
		crossover = 0.7;
		mutation = 0.1;
		stability = 5;

		printf("Instance: %s. Using default genetic algorithm params.\nPopulation size = %d\nCrossover percentage = %.2f\nMutation probability = %.2f\nMax stable instances: %d\n", instance.c_str(), pop, crossover, mutation, stability);

	} else if (argc == 6) { // all params passed: parse 'em
		instance = argv[1];
		pop = stoi(argv[2]);
		crossover = stof(argv[3]);
		mutation = stof(argv[4]);
		stability = stoi(argv[5]);

		printf("Instance: %s. Population size = %d\nCrossover percentage = %.2f\nMutation probability = %.2f\nMax stable instances = %d\n", instance.c_str(), pop, crossover, mutation, stability);

	} else { // wrong number of params
		error("Wrong number of command line params.\nCORRECT: gen.exe <instance> <population_size> <crossover> <mutation> <stability>\nEXAMPLE: ./gen keller5 60 0.7 0.1 5\n");
	}


	printf("\nConstructing graph %s from %s.txt...\n", instance.c_str(), instance.c_str());
	Graph g = Graph(GRAPHS_PATH, instance + ".txt");

	printf("Graph ready.\n");

	TIMESTAMP t0 = time();
	BINARY_LIST mvc = genetic_algorithm(g, pop, crossover, mutation, stability, true);
	long runtime = elapsed_time(t0);
	int mvc_size = cost(mvc);

	printf("[Graph %s - size %d] Returned solution: %d vertices (%dms).\n", instance.c_str(), g.get_n(), mvc_size, runtime);

	/* output file */
	ofstream outfile;
	string outfilename = "output.csv";
	outfile.open(outfilename, std::ofstream::out | std::ofstream::app);

	if (outfile.is_open()) {
		outfile << instance.c_str() << "," << pop << "," << crossover << "," << mutation << "," << stability << "," << runtime << "," << mvc_size << endl;
		printf("Test result written to output.csv.\n");
		outfile.close();
	} else {
		printf("Could not create/open output.csv to append results.\n");
	}

	return 0;
}