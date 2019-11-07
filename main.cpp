#include "main.h"
#include "heuristics.h"

#define GRAPHS_PATH "test_data/clique_complements/"

typedef struct instance {
	Graph graph; // actual class instance
	string name; // instance name
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

	outfile << "instance,alpha,max_time,max_it,runtime,mvc\n"; // csv header
	outfile.close();


	for (auto it = test_filenames.begin(); it != test_filenames.end(); it++) { // only test instances
		if (*it != "p_hat300-2.clq-compliment.txt") continue; // debugging!

		TIMESTAMP t0 = time();
		INSTANCE inst{ Graph(GRAPHS_PATH, *it) };
		
		inst.construction_time = elapsed_time(t0);
		inst.name = inst.graph.get_name();

		printf("CURRENT INSTANCE: %s\n", inst.name.c_str());

		/* run method and store result */
		float grasp_alpha = 0.5;
		int max_grasp_time = 3*60*1000; // 3min
		int max_grasp_iterations = 5;
		t0 = time();
		inst.mvc = grasp(inst.graph, grasp_alpha, max_grasp_time, max_grasp_iterations, true);
		long dt = elapsed_time(t0);

		if (inst.mvc != INT_SET_NULL) { // it worked!
			inst.mvc_size = inst.mvc.size();
			inst.runtime = dt;
			
			outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
			
			// grasp: instance,alpha,max_time,max_it,runtime,mvc
			outfile << inst.name << "," << grasp_alpha << "," << max_grasp_time << "," << max_grasp_iterations << "," << inst.runtime << "," << inst.mvc_size << "\n";
			outfile.close();

			printf("RESULT RECORDED: '%s', MVC of size %d. (%dms)\n", inst.name.c_str(), inst.mvc_size, inst.runtime);

			exit(1); // debugging
		}

		else {
			error("Method returned error - " + inst.name + " possibly didn't pass vertex cover verification D:");
		}
	}

	if (outfile.is_open()) outfile.close();
	return 0;
}