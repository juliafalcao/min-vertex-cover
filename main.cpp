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
	outfile.open("results/rm-greedy.csv", std::ofstream::out | std::ofstream::app);

	if (!outfile.is_open()) {
		error("Error opening file to print results.\n");
	}

	outfile << "instance,N,M,construction,runtime,rm-greedy-100\n"; // csv header

	for (auto it = filenames.begin(); it != filenames.end(); it++) {
		TIMESTAMP t0 = time();
		INSTANCE inst{
			Graph(GRAPHS_PATH, *it)
		};
		
		inst.construction_time = elapsed_time(t0);		
		inst.name = inst.graph.get_name();

		// printf("Instance '%s' ready: %d nodes, %d edges, built in %lms\n", inst.name, inst.graph.get_n(), inst.graph.get_m(), inst.construction_time);

		/* run heuristic */
		t0 = time();
		// inst.mvc = simple_greedy(inst.graph); // simple greedy, no randomization
		inst.mvc = random_multistart_simple_greedy(inst.graph, 100); // random multistart, shuffles array
		long dt = elapsed_time(t0);

		auto first = inst.mvc.begin();
		if (*first != -1) { // it worked!
			inst.mvc_size = inst.mvc.size();
			inst.runtime = dt;

			// printf("%s\t%d\t%d\t%ld\t%d\t%ld\n", inst.name.c_str(), inst.graph.get_n(), inst.graph.get_m(), inst.construction_time, inst.runtime, inst.mvc_size);
			
			outfile << inst.name << "," << inst.graph.get_n() << "," << inst.graph.get_m() << "," << inst.construction_time << "," << inst.runtime << "," << inst.mvc_size << "\n";

		}

		else {
			cout << inst.name << " - MNF returned error :(" << endl;
		}
	}

	if (outfile.is_open()) outfile.close();

	return 0;
}