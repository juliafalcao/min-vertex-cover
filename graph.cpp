#include "graph.h"
#include "utils.h"


Graph::Graph(string path, string filename, bool directed) {
	this->directed = directed;

	int s = this->file_suffix_to_remove.length();
	this->name = filename/*.substr(0, filename.length()-s)*/;

	make_graph(path, filename, this->n);
	this->m = this->edges.size();

	cout << "Graph '" << this->name << "' with " << this->n << " vertices constructed successfully.";
}

/*
print adjacencies list
*/
void Graph::print_adj() {
	for (auto it = this->adj.begin(); it != this->adj.end(); it++) {
		cout << it->first << ": { ";
		
		for (auto v_it = it->second.begin(); v_it != it->second.end(); v_it++) {
			cout << *v_it << " ";
		}

		cout << "}" << endl;
	}
}

/*
print edges list
*/
void Graph::print_edges() {
	cout << "{ ";
	for (auto it = this->edges.begin(); it != this->edges.end(); it++) {
		cout << "(" << it->first << ", " << it->second << ") ";
	}
	cout << "}" << endl;
}

/*
function that opens graph file and builds its adjacency or edges list
*/
void Graph::make_graph(string path, string filename, int &n_vertices) {
	string line;
	string filepath = path + filename;
	ifstream file(filepath, ifstream::binary);

	if (!file.is_open()) error("Não foi possível abrir o arquivo '" + filename + "'.");

	int n = 0; // nodes
	int m = 0; // edges
	int i = 0, i0 = 0, i1 = 0;
	int a = 0, b = 0; // edge vertices
	string buffer;
	bool line0 = true;
	ADJ_PAIR_LIST adj;
	// INT_PAIR_LIST edges;
	this->edges = {};

	while (getline(file, line)) {
		if (file.eof()) break;

		i = find(line, " ", 0); // first
		if (i == -1) error("Find() retornou -1.");

		buffer = line.substr(0, i);

		if (buffer != "p") error("Primeiro caracter '" + buffer + "' deveria ser 'p'.");

		int i0 = i;
		i = find(line, " ", i0+1);
		if (i == -1) error("Find() retornou -1.");

		buffer = line.substr(i0+1, i-i0-1);
		
		if (line0) { // "p edge 200 5066"
			if (buffer != "edge" && buffer != "col") {
				error("Buffer '" + buffer + "' deveria conter 'edge' ou 'col'.");
			}
			
			else { // parse line0
				i1 = find(line, " ", i+1);
				buffer = line.substr(i+1, i1-i-1);
				n = stoi(buffer); // number of vertices
				n_vertices = n;

				/* parse edge count here if needed */

				// initialize adj list
				for (int v = 1; v <= n; v++) {
					INT_LIST v_adj = {};
					INT_LIST_PAIR v_adj_pair = make_pair(v, v_adj);
					adj.push_back(v_adj_pair);
				}
			}
		}

		else { // "p 196 197"
			a = stoi(buffer);
			i0 = i;

			buffer = line.substr(i0+1, line.length()-i0);
			b = stoi(buffer);

			// add edge to adjacency list
			for (auto it = adj.begin(); it != adj.end(); it++) {
				if (it->first == a) {
					it->second.push_back(b);
				}

				else if (it->first == b && !this->directed) {
					it->second.push_back(a);
				}
			}

			// add edge to edges list
			INT_PAIR edge = make_pair(a, b);
			this->edges.push_back(edge);
		}

		line0 = false;
	} // while

	file.close();

	this->adj = adj; // TODO: edit this->adj inplace instead of saving later
	return;
} // make_graph

void Graph::sort_adj_by_most_neighbors(void) {
	sort(this->adj.begin(), this->adj.end(), most_values_comparator);
}

ADJ_PAIR_LIST Graph::get_adj_copy() { // TODO: move to utils.cpp
	int v;
	INT_LIST v_adj = {};
	INT_LIST_PAIR v_adj_pair;
	ADJ_PAIR_LIST copy;

	for (auto it = this->adj.begin(); it != this->adj.end(); it++) {
		v = it->first;
		v_adj = {};
		
		for (auto vit = it->second.begin(); vit != it->second.end(); vit++) {
			v_adj.push_back(*vit);
		}

		v_adj_pair = make_pair(v, v_adj);
		copy.push_back(v_adj_pair);
	}

	return copy;
}

int Graph::get_m(void) {
	return this->m;
}