#include "main.h"

#define GRAPHS_PATH "test_data/clique_complements/"
#define NOT_FOUND -1

int find(INT_LIST vec, int x) { // TODO: move to utils.cpp
	for (int i = 0; i < vec.size(); i++) { // TODO: binary search???
		if (vec[i] == x) return i;
	}

	return NOT_FOUND;
}

INT_LIST subtract(INT_LIST a, INT_LIST b) { // TODO: move to utils.cpp
	INT_LIST new_a = {};

	for (auto it = a.begin(); it != a.end(); it++) {
		if (find(b, *it) == NOT_FOUND) {
			new_a.push_back(*it); // add to new_a elements not in b
		}
	}

	return new_a;
}

void print_adj(ADJ_PAIR_LIST adj) { // TODO: move to utils.cpp
	for (auto it = adj.begin(); it != adj.end(); it++) {
		cout << it->first << ": { ";
		
		for (auto v_it = it->second.begin(); v_it != it->second.end(); v_it++) {
			cout << *v_it << " ";
		}

		cout << "}" << endl;
	}
}

/*
first constructive heuristic
*/
void most_neighbors_first(Graph &g) {

	/* sort adj list by most neighbors first, name second */
	g.sort_adj_by_most_neighbors();
	
	ADJ_PAIR_LIST adjc = g.get_adj_copy(); // this i can manipulate
	int total_edges = g.get_m();
	int covered_edges = 0;
	int newly_covered_edges = 0;

	
	INT_SET Vc; // vertex cover!
	int v; // current vertex
	INT_LIST v_adj; // current vertex's adjacencies
	INT_LIST s_v_adj; // subsequent vertex's adjacencies
	
	cout << "Original adj list:" << endl;
	print_adj(adjc);
	cout << endl;

	for (auto it = adjc.begin(); it != adjc.end(); it++) { // iterate through ordered adj list
		v = it->first;
		v_adj = it->second;

		Vc.insert(v);
		cout << "Added " << v << " to vertex cover." << endl;
		
		for (auto s_it = it; s_it != adjc.end(); s_it++) {
			// remove new vertex added to cover from all the remaining vertices' adjacency lists
			s_v_adj = s_it->second;
			INT_LIST::iterator pos = find(s_v_adj.begin(), s_v_adj.end(), v);
			if (pos != s_v_adj.end()) s_v_adj.erase(pos);

			s_it->second = s_v_adj; // necessary?
			newly_covered_edges = s_it->second.size();
		}

		covered_edges += newly_covered_edges; // this isn't working
		// TODO: make sure this won't count the same edge twice

		if (covered_edges == total_edges) {
			cout << "Finished! Vertex cover: ";
			for (auto it = Vc.begin(); it != Vc.end(); it++) cout << *it << " ";
			break;
		} else if (covered_edges > total_edges) {
			error("oops");
		}


		// reorder the rest of adjc here somehow so that next adj list minus the vertex added to Vc is still the largest
		sort(it, adjc.end(), most_values_comparator); // WHAT

		cout << "New adj list:" << endl;
		print_adj(adjc);
	}

	return;
}

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