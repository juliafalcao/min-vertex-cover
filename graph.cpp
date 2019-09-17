#include "graph.h"
#include "utils.h"


Graph::Graph(string path, string filename, bool directed) {
	this->directed = directed;

	int s = this->file_suffix_to_remove.length();
	this->name = filename.substr(0, filename.length()-s);

	this->adj = make_graph(path, filename, this->n);
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
function that opens graph file and builds its adjacency list
*/
ADJ_PAIR_LIST Graph::make_graph(string path, string filename, int &n_vertices) {
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
		}

		line0 = false;
	} // while

	file.close();
	return adj;
} // make_graph