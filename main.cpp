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
verification algorithm
*/
bool verify_vertex_cover(Graph &g, INT_SET cover) {
	for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
		if (find(cover.begin(), cover.end(), it->first) == cover.end() 
		&& find(cover.begin(), cover.end(), it->second) == cover.end()) {
			// neither vertex is in vertex cover: edge isn't covered
			return false;
		}
	}

	return true;
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
	int new_edges = 0;

	
	INT_SET Vc; // vertex cover!
	int v; // current vertex
	INT_LIST v_adj; // current vertex's adjacencies list
	INT_LIST s_v_adj; // subsequent vertices' adjacencies lists
	
	cout << "Original adj list:" << endl;
	print_adj(adjc);
	cout << endl;

	for (auto it = adjc.begin(); it != adjc.end(); it++) { // iterate through ordered adj list
		v = it->first;
		v_adj = it->second;

		for (auto a_it = v_adj.begin(); a_it != v_adj.end(); a_it++) {
			int x = *a_it;
			// iterate through v's adjacencies to see if it covers any new edges
			if (find(Vc.begin(), Vc.end(), x) == Vc.end()) {
				newly_covered_edges++;
				// count (v, x) in as new edge if vertex x is not in Vc
			}
		}

		if (newly_covered_edges) {
			// if v's adjacencies cover any new edges
			Vc.insert(v);
			cout << "Added " << v << " to vertex cover." << endl;
			covered_edges += newly_covered_edges;

			if (covered_edges == total_edges) { // stop condition
				if (verify_vertex_cover(g, Vc)) {
					cout << "Finished! Vertex cover: ";
					for (auto it = Vc.begin(); it != Vc.end(); it++) cout << *it << " ";
					break;
				} else {
					error("Algorithm finished but Vc didn't pass vertex cover verification :(");
				}
			}
			
			else if (covered_edges > total_edges) { // done fucked up
				error("oops");
			}

			else { // prepare for next iteration
				newly_covered_edges = 0;

				for (auto s_it = it; s_it != adjc.end(); s_it++) {
				// iterate through all the remaining vertices's adjacencies to remove v
				s_v_adj = s_it->second;
				INT_LIST::iterator pos = find(s_v_adj.begin(), s_v_adj.end(), v);
				if (pos != s_v_adj.end()) s_v_adj.erase(pos);

				s_it->second = s_v_adj; // necessary?
				}

				sort(it, adjc.end(), most_values_comparator);
				// reorder the rest of adjc so that the modified adj lists are still sorted by largest to smallest

				cout << "New adj list:" << endl;
				print_adj(adjc);
			}
		}

		// else if v wouldn't cover any new edges: 
		
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