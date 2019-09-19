#include "heuristics.h"


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
first constructive heuristic: greedy adaptive?
*/
INT_SET most_neighbors_first(Graph &g, bool debug_mode) {
	
	ADJ_PAIR_LIST adjc = g.get_adj_copy(); // to manipulate without altering g

	sort(adjc.begin(), adjc.end(), most_values_comparator); // sort by adj lists with most values first

	int total_edges = g.get_m();
	int covered_edges = 0;
	int newly_covered_edges = 0;
	int new_edges = 0;

	INT_SET Vc; // vertex cover!
	int v; // current vertex
	INT_LIST v_adj; // current vertex's adjacencies list
	INT_LIST s_v_adj; // subsequent vertices' adjacencies lists
	
	if (debug_mode) {
		cout << "Original adj list:" << endl;
		print_adj(adjc);
		cout << endl;
	}

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
			if (debug_mode) cout << "Added " << v << " to vertex cover." << endl;
			covered_edges += newly_covered_edges;

			if (covered_edges == total_edges) { // stop condition
				if (verify_vertex_cover(g, Vc)) {
					if (debug_mode) {
						cout << "Finished! Vertex cover: ";
						for (auto it = Vc.begin(); it != Vc.end(); it++) cout << *it << " ";
					}
					
					break;
				} else {
					if (debug_mode) cout << "Algorithm finished but Vc didn't pass vertex cover verification :(" << endl;
					return INT_SET{-1};
				}
			}
			
			else if (covered_edges > total_edges) { // done fucked up
				if (debug_mode) cout << "Covered edges count surpassed total edges :O" << endl;
				return INT_SET{-1};
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

				if (debug_mode) {
					cout << "New adj list:" << endl;
					print_adj(adjc);
				}
			}
		}

		// else if v wouldn't cover any new edges: onto the next one
		if (debug_mode) printf("Vertex %d doesn't cover any new edges; continuing.", v);
	}

	return Vc;
}