#include "main.h"
#include "heuristics.h"

#define GRAPHS_PATH "test_data/clique_complements/"

typedef struct instance {
	Graph graph; // actual class instance
	string name;
	int construction_time; // time to build graph, in ms
	bool train; // true for training instance, false for validation instance
	int runtime; // runtime
	INT_SET mvc; // the vertex cover found
	int mvc_size; // size of vertex cover
} INSTANCE;

typedef vector<INSTANCE> INSTANCE_LIST;


int main(void) {
	// get all filenames from dir
	STR_LIST filenames = list_dir(GRAPHS_PATH);
	int n_graphs = filenames.size();

	/* separate into training and testing sets */
	// int n_test = floor(0.2 * n_graphs); // 20% for testing
	// int n_train = n_graphs - n_test;
	// STR_LIST train, test;

	// srand(1); // seed

	// while (train.size() < n_train) { // make train set
	// 	string gf = filenames[rand() % n_graphs];

	// 	if (find(train.begin(), train.end(), gf) == train.end()) {
	// 		// graph filename not yet in train set, add it
	// 		train.push_back(gf);
	// 	}
	// }

	// for (auto it = filenames.begin(); it != filenames.end(); it++) { // make test set
	// 	if (find(train.begin(), train.end(), *it) == train.end()) {
	// 		// graph filename not in train set, add to test set
	// 		test.push_back(*it);
	// 	}
	// }

	// printf("Separated %d graphs for training and %d for validation.\n", n_train, n_test);

	/* load actual instances and start heuristics */
	INSTANCE_LIST instances;
	ofstream outfile;
	outfile.open("results/ls-1st.csv", std::ofstream::out | std::ofstream::app);

	if (!outfile.is_open()) {
		error("Error opening file to print results.\n");
	}

	outfile << "instance,N,M,runtime,search-iterations,mvc\n"; // csv header

	for (auto it = filenames.begin(); it != filenames.end(); it++) {
		TIMESTAMP t0 = time();
		INSTANCE inst{
			Graph(GRAPHS_PATH, *it)
		};
		
		inst.construction_time = elapsed_time(t0);		
		inst.name = inst.graph.get_name();

		// if (inst.name != "brock200_1") continue; // DEBUG

		/* run method and store result */
		t0 = time();
		// inst.mvc = simple_greedy(inst.graph); // simple greedy
		// inst.mvc = random_multistart_simple_greedy(inst.graph, 10, true); // random multistart
		
		int max_it = 300, search_iterations = 0;
		inst.mvc = local_search(inst.graph, max_it, search_iterations);

		long dt = elapsed_time(t0);

		if (inst.mvc != INT_SET_NULL) { // it worked!
			inst.mvc_size = inst.mvc.size();
			inst.runtime = dt;
			
			outfile << inst.name << "," << inst.graph.get_n() << "," << inst.graph.get_m() << "," << inst.runtime << "," << search_iterations << "," << inst.mvc_size << "\n";

			// outfile.close(); // DEBUG
			// exit(1); // DEBUG
		}

		else {
			outfile.close();
			error("Method returned error - " + inst.name + " possibly didn't pass vertex cover verification D:");
		}
	}

	if (outfile.is_open()) outfile.close();

	return 0;
}