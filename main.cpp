#include "main.h"
#include "heuristics.h"

#define GRAPHS_PATH "test_data/clique_complements/"


int main(void) {
	// get all filenames from dir
	vector<string> filenames = list_dir(GRAPHS_PATH);
	int n_graphs = filenames.size();
	int n_test = floor(0.2 * n_graphs); // separate 20% for testing
	int n_train = n_graphs - n_test;
	cout << "Separated " << n_train << " graphs for training and " << n_test << " graphs for validation." << endl;

	// load actual graphs
	vector<Graph> graphs = {};

	for (auto it = filenames.begin(); it != filenames.end(); it++) {
		cout << "Opening file: " << *it << endl;
		TIMESTAMP t0 = time();
		// Graph g(GRAPHS_PATH, *it);
		Graph gt("", "test_graph.txt");

		long dt = elapsed_time(t0);
		cout << " [" << dt << "ms]" << endl;

		// gt.print_edges();
		// gt.print_adj();

		most_neighbors_first(gt);

		// graphs.push_back(g);
		break;
	}

	cout << "Finished." << endl;


	return 0;
}