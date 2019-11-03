#include "graph.h"
#include "utils.h"


Graph::Graph(string path, string filename, bool directed) {
	this->directed = directed;

	int s = this->file_suffix_to_remove.length();
	this->name = filename.substr(0, filename.length()-s);

	make_graph(path, filename, this->n);
	this->m = this->edges.size();
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
	int i = 0, i1 = 0;
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

ADJ_PAIR_LIST Graph::get_adj_copy(void) { // TODO: move to utils.cpp
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

INT_PAIR_LIST Graph::get_edges_copy(void) {
	int a, b;
	INT_PAIR_LIST copy;

	for (auto it = this->edges.begin(); it != this->edges.end(); it++) {
		a = it->first;
		b = it->second;
		INT_PAIR edge = make_pair(a, b);
		copy.push_back(edge);
	}

	return copy;
}



int Graph::get_n(void) {
	return this->n;
}

int Graph::get_m(void) {
	return this->m;
}

string Graph::get_name(void) {
	return this->name;
}

INT_LIST Graph::get_vertex_adj(int vertex) {
	for (auto it = this->adj.begin(); it != this->adj.end(); it++) {
		if (it->first == vertex) {
			return it->second;
		}
	}

	return {-1};
}

int Graph::degree(int vertex) {
	INT_LIST v_adj = get_vertex_adj(vertex);
	return v_adj.size(); // the size of this vertex's adjacency list
}

INT_LIST Graph::get_vertex_list(void) {
	INT_LIST vertices = {};
	for (auto it = this->adj.begin(); it != this->adj.end(); it++) {
		vertices.push_back(it->first);
	}

	return vertices;
}

/*
comparer function that returns whether v1 should be placed before v2 or not
*/
bool Graph::compare_degree_reverse(int v1, int v2) {
	int d1 = this->degree(v1);
	int d2 = this->degree(v2);

	return (d1 > d2); // <: ascending order, >: descending order
}

INT_LIST Graph::sort_vertices_by_higher_degree(INT_LIST vertices) {
	sort(vertices.begin(), vertices.end(),
		[this](int v1, int v2) { return this->compare_degree_reverse(v1, v2); } // c++ lambda!
	);

	return vertices;
}

/*
functions to get the minimum and maximum degree values for the vertices in the graph
*/
int Graph::min_degree(void) {
	int min_degree = INT_MAX, degree = -1;

	for (auto it = this->adj.begin(); it != this->adj.end(); it++) {
		degree = this->degree(it->first); // vertex degree

		if (degree < min_degree) {
			min_degree = degree;
		}
	}

	return min_degree;
}

int Graph::max_degree(void) {
	int max_degree = 0, degree = -1;

	for (auto it = this->adj.begin(); it != this->adj.end(); it++) {
		degree = this->degree(it->first);

		if (degree > max_degree) {
			max_degree = degree;
		}
	}

	return max_degree;
}