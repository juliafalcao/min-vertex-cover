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

	// for debugging
	// auto ini = find(filenames.begin(), filenames.end(), "MANN_a9.clq-compliment.txt");
	/*remove!*/ // filenames = {"c-fat200-1.clq-compliment.txt", "c-fat200-2.clq-compliment.txt", "c-fat200-5.clq-compliment.txt", "c-fat500-1.clq-compliment.txt", "c-fat500-10.clq-compliment.txt", "c-fat500-5.clq-compliment.txt", "p_hat1500-1.clq-compliment.txt"};

	for (auto it = test_filenames.begin(); it != test_filenames.end(); it++) { // only testing
		TIMESTAMP t0 = time();
		INSTANCE inst{ Graph(GRAPHS_PATH, *it) };
		
		inst.construction_time = elapsed_time(t0);
		inst.name = inst.graph.get_name();

		printf("CURRENT INSTANCE: %s\n", inst.name.c_str());

		/* run method and store result */
		// int max_it = 300, max_it_1st = 300, search_iterations = 0, improvement = 0;
		float grasp_alpha = 0.5;
		int max_grasp_time = 5*60*1000; // 5min
		int max_grasp_iterations = 100;
		t0 = time();
		// inst.mvc = rm_local_search(inst.graph, "first", 20, max_it, max_it_1st, improvement, true);
		inst.mvc = grasp(inst.graph, grasp_alpha, max_grasp_time, max_grasp_iterations, true);
		long dt = elapsed_time(t0); // TODO: recalc

		if (inst.mvc != INT_SET_NULL) { // it worked!
			inst.mvc_size = inst.mvc.size();
			inst.runtime = dt;
			
			outfile.open(outfilename, std::ofstream::out | std::ofstream::app);
			// outfile << inst.name << "," << inst.graph.get_n() << "," << inst.graph.get_m() << "," << inst.runtime << "," << search_iterations << "," << inst.mvc_size << "\n";
			
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