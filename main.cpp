#include "main.h"
#include "heuristics.h"

#define GRAPHS_PATH "test_data/clique_complements/"

typedef struct instance {
	Graph graph; // actual class instance
	string name; // instance name
	int runtime; // runtime
	INT_SET mvc; // the vertex cover found
	int mvc_size; // size of vertex cover

	// for grasp tests: all vectors below must be the same length!
	FLOAT_LIST alphas;
	INT_LIST runtimes;
	INT_LIST mvc_sizes;
} INSTANCE;

typedef vector<INSTANCE> INSTANCE_LIST;

int main_grasp_comparison(void) {
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
	string outfilename = "results/grasp-tests.csv";
	outfile.open(outfilename, std::ofstream::out | std::ofstream::app);

	if (!outfile.is_open()) {
		error("Error opening file to print results.\n");
	}

	outfile << "instance,max_time,max_it,alpha,runtime,mvc_size\n"; // csv header
	outfile.close();


	for (auto it = test_filenames.begin(); it != test_filenames.end(); it++) { // only test instances
		// if (*it != "p_hat300-2.clq-compliment.txt") continue; // debugging!

		INSTANCE inst{ Graph(GRAPHS_PATH, *it) };
		inst.name = inst.graph.get_name();

		printf("CURRENT INSTANCE: %s\n", inst.name.c_str());

		/* run method and store result */
		inst.alphas = {0.3, 0.5, 0.7};
		inst.runtimes = {};
		inst.mvc_sizes = {};
		int max_grasp_time = 2*60*1000; // 2min
		int max_grasp_iterations = 30;
		INT_SET mvc;

		// loop through candidate alphas and store results
		for (int i = 0; i < inst.alphas.size(); i++) {
			float grasp_alpha = inst.alphas[i];
			TIMESTAMP t0 = time();
			mvc = grasp(inst.graph, grasp_alpha, max_grasp_time, max_grasp_iterations, false);
			inst.runtimes.push_back(elapsed_time(t0));
			inst.mvc_sizes.push_back(mvc.size());
			printf("Alpha = %.1f: Found best MVC of size %d.\n", grasp_alpha, mvc.size());

			// record in file
			outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
			outfile << inst.name.c_str() << "," << max_grasp_time << "," << max_grasp_iterations << "," << grasp_alpha << "," << inst.runtimes[i] << "," << inst.mvc_sizes[i] << endl;
			outfile.close();
		}

		// if (inst.mvc != INT_SET_NULL) { // it worked!
		// 	inst.mvc_size = inst.mvc.size();
		// 	inst.runtime = dt;
		// }



		// } else {
		// 	error("Method returned error - " + inst.name + " possibly didn't pass vertex cover verification D:");
		// }
	}

	if (outfile.is_open()) outfile.close();
	return 0;
}

/* path relinking shit */
int main(void) {
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

	// outfile << "instance,max_time,max_it,alpha,runtime,mvc_size\n"; // csv header
	// outfile.close();


	for (auto it = test_filenames.begin(); it != test_filenames.end(); it++) { // only test instances
		if (*it != "p_hat300-2.clq-compliment.txt") continue; // debugging!

		INSTANCE inst{ Graph(GRAPHS_PATH, *it) };
		inst.name = inst.graph.get_name();

		printf("CURRENT INSTANCE: %s\n", inst.name.c_str());

		/* run method and store result */
		inst.alphas = {0.3, 0.5, 0.7};
		inst.runtimes = {};
		inst.mvc_sizes = {};
		int max_grasp_time = 2*60*1000; // 2min
		int max_grasp_iterations = 30;
		

	}

	return 0;
}