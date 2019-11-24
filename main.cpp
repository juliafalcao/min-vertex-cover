#include "main.h"

#define GRAPHS_PATH "test_data/clique_complements/"

typedef struct instance {
	Graph graph; // actual class instance
	string name; // instance name
	int runtime; // runtime
	INT_SET mvc; // the vertex cover found
	int mvc_size; // size of vertex cover
	int target; // target result
} INSTANCE;

typedef vector<INSTANCE> INSTANCE_LIST;


int main(void) {

	string test_instance = "johnson8-2-4.clq-compliment.txt";

	Graph g = Graph(GRAPHS_PATH, test_instance);

	BINARY_SOLUTIONS population = initialize_population(g, 10);
	srand(time(0));

	for (int i = 0; i < 10; i++) {
		BINARY_LIST s = roulette_wheel_selection(population);
		printf("Roulette wheel selection: size = %d, fitness = %.5f\n", cost(s), fitness(s));
		print(s);
	}

	return 0;
}


int old_main(void) {
	// get all filenames from dir
	STR_LIST filenames = list_dir(GRAPHS_PATH);
	STR_LIST test_filenames = {};
	
	// get filenames of separated test instances
	ifstream file;
	file.open("test_filenames.txt", std::ifstream::in);
    string line;
    while (getline(file, line)) {
		test_filenames.push_back(line);
	}

	/* load actual instances and start heuristics */
	INSTANCE_LIST instances;
	ofstream outfile;
	string outfilename = "results/grasp-pr.csv";
	outfile.open(outfilename, std::ofstream::out | std::ofstream::app);

	if (!outfile.is_open()) {
		error("Error opening file to print results.\n");
	}

	outfile << "instance,max_time,max_it,alpha,runtime,mvc_size\n"; // csv header
	outfile.close();

	auto first = find(filenames.begin(), filenames.end(), "p_hat1500-3.clq-compliment.txt");
	// auto first = filenames.begin();

	for (auto it = first; it != filenames.end(); it++) { // all instances
		// while (*it != "c-fat200-5.clq-compliment.txt") continue; // debugging!

		INSTANCE inst{ Graph(GRAPHS_PATH, *it) };
		inst.name = inst.graph.get_name();

		printf("CURRENT INSTANCE: %s\n", inst.name.c_str());

		/* run method and store result */
		float grasp_alpha = 0.3;
		int max_grasp_time = 2*60*1000; // 2min
		int max_grasp_iterations = 30;
		int max_elite_set_size = 5;
	
		TIMESTAMP t0 = time();
		inst.mvc = grasp_pr(inst.graph, grasp_alpha, max_grasp_time, max_grasp_iterations, max_elite_set_size, true);
		// inst.mvc = grasp(inst.graph, grasp_alpha, max_grasp_time, max_grasp_iterations, true);
		long dt = elapsed_time(t0);

		if (inst.mvc != INT_SET_NULL) { // it worked!
			inst.mvc_size = inst.mvc.size();
			inst.runtime = dt;
			printf("Method returned solution of %d vertices.\n", inst.mvc_size);
		} else {
			error("Method returned error - " + inst.name + " possibly didn't pass vertex cover verification D:");
		}

		// record in file
		outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
		outfile << inst.name.c_str() << "," << max_grasp_time << "," << max_grasp_iterations << "," << grasp_alpha << "," << inst.runtime << "," << inst.mvc_size << endl;
		outfile.close();
	}

	if (outfile.is_open()) outfile.close();
	return 0;
}

/* to test stuff */
int test_main(void) {
	// get all filenames from dir
	// STR_LIST filenames = list_dir(GRAPHS_PATH);
	STR_LIST test_filenames = {};
	
	// get filenames of separated test instances
	ifstream file;
	file.open("test_filenames.txt", std::ifstream::in);
    string line;
    while (getline(file, line)) {
		test_filenames.push_back(line);
	}

	/* load actual instances and start heuristics */
	INSTANCE_LIST instances;
	ofstream outfile;
	string outfilename = "results/pr-tests.csv";
	// outfile.open(outfilename, std::ofstream::out | std::ofstream::app);

	// if (!outfile.is_open()) {
		// error("Error opening file to print results.\n");
	// }

	outfile << "instance,max_time,max_it,alpha,runtime,mvc_size\n"; // csv header
	// outfile.close();


	for (auto it = test_filenames.begin(); it != test_filenames.end(); it++) { // only test instances
		// if (*it != "p_hat300-2.clq-compliment.txt") continue; // debugging!

		/* update elite set */

		Graph tg = Graph({0, 1, 2, 3, 4});
		INT_SET s1 = {0, 1, 2, 3};
		INT_SET s2 = {0, 1, 2};
		INT_SET s3 = {1, 2, 3};
		INT_SET sc = {2, 3, 4};
		INT_SET random_elite = {};

		SOLUTION_SET elite_set = {};
		elite_set.insert(s1);
		elite_set.insert(s2);
		elite_set.insert(s3);
		

		srand(time(NULL));
		if (!elite_set.empty()) {
			/* choose random solution from elite set */
			if (elite_set.size() == 1) random_elite = *elite_set.begin();
			else {
				auto it = elite_set.begin();
				advance(it, rand() % elite_set.size());
				random_elite = *it;
			}
		}

		printf("RANDOM ELITE SOLUTION: ");
		print(random_elite);
	}

	return 0;
}