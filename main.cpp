#include "main.h"

#define GRAPHS_PATH "test_data/clique_complements/"

int main(void) {
	STR_LIST test_filenames = {};
	ifstream file;
	file.open("test_filenames.txt", std::ifstream::in);
    string line;
    while (getline(file, line))	test_filenames.push_back(line);

	/* load actual instances and run method */
	ofstream outfile;
	string outfilename = "results/genetic.csv";
	outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
	if (!outfile.is_open()) error("Error opening CSV file to output results.\n");

	outfile << "instance,population,crossover,mutation,stability,runtime,result\n"; // csv header
	outfile.close();

	string DEBUG = ""; // instance name to test
	for (auto it = test_filenames.begin(); it != test_filenames.end(); it++) { // all instances
		if (DEBUG != "" && *it != DEBUG) continue;

		Graph g = Graph(GRAPHS_PATH, *it);
		string instance = g.get_name();
		printf("\nCURRENT INSTANCE: %s\n", instance.c_str());

		/* run method and store result */
		int pop = 50;
		float crossover = 0.7;
		float mutation = 0.1;
		int stability = 5;

		TIMESTAMP t0 = time();
		BINARY_LIST mvc = genetic_algorithm(g, pop, crossover, mutation, stability, false);
		long runtime = elapsed_time(t0);

		if (mvc == BINARY_LIST_NULL) error("Method returned error -> " + instance + " possibly didn't pass vertex cover verification.");
		
		int mvc_size = cost(mvc);
		printf("[%s - size %d] Returned solution: %d vertices.\n", instance.c_str(), g.get_n(), mvc_size);

		// record in csv file
		outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
		char* buffer;
		sprintf(buffer, "%s,%d,%.2f,%.2f,%d,%d,%d\n", instance.c_str(), pop, crossover, mutation, stability, runtime, mvc_size);
		outfile << buffer;

		outfile.close();
	}


	if (outfile.is_open()) outfile.close();
	return 0;

	return 0;
}

int grasp_main(void) {
	// get filenames of all instances from dir
	STR_LIST filenames = list_dir(GRAPHS_PATH);
	
	// get filenames of separated test instances
	STR_LIST test_filenames = {};
	ifstream file;
	file.open("test_filenames.txt", std::ifstream::in);
    string line;
    while (getline(file, line))	test_filenames.push_back(line);

	/* load actual instances and run method */
	ofstream outfile;
	string outfilename = "results/grasp-pr-2.csv";
	outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
	if (!outfile.is_open()) error("Error opening CSV file to output results.\n");

	outfile << "instance,max_time,max_it,alpha,max_elite,runtime,mvc_size\n"; // csv header
	outfile.close();

	string DEBUG = ""; // instance name to test
	for (auto it = filenames.begin(); it != filenames.end(); it++) { // all instances
		if (DEBUG != "" && *it != DEBUG) continue;

		Graph g = Graph(GRAPHS_PATH, *it);
		string instance = g.get_name();
		printf("\nCURRENT INSTANCE: %s\n", instance.c_str());

		/* run method and store result */
		float alpha = 0.3;
		int max_time = 2*60*1000; // 2min
		int max_it = 30;
		int max_elite = 5;
	
		TIMESTAMP t0 = time();
		INT_SET mvc = grasp_pr(g, alpha, max_time, max_it, max_elite, true);
		// INT_SET mvc = grasp(g, alpha, max_time, max_it, true);
		long runtime = elapsed_time(t0);

		if (mvc == INT_SET_NULL) error("Method returned error -> " + instance + " possibly didn't pass vertex cover verification.");
		
		int mvc_size = mvc.size();
		printf("Returned MVC: %d vertices.\n", mvc_size);

		// record in csv file
		outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
		outfile << instance.c_str() << "," << max_time << "," << max_it << "," << alpha << "," << max_elite << "," << runtime << "," << mvc_size << endl;
		outfile.close();
	}

	if (outfile.is_open()) outfile.close();
	return 0;
}