#include "graph.h"
#include "utils.h"


Graph::Graph(string path, string filename, bool directed) {
	this->directed = directed;

	int s = this->file_suffix_to_remove.length();
	this->name = filename.substr(0, filename.length()-s);

	make_graph(path, filename);
	this->m = this->edges.size();
	this->vertices = this->get_vertex_list();
	printf("Vertex list: ");
	print(this->vertices);
}

Graph::Graph(INT_SET test_vertices) {
	this->n = test_vertices.size();
	this->m = -1;
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
function that opens graph file and builds its edges list
*/
void Graph::make_graph(string path, string filename) {
	string line;
	string filepath = path + filename;
	ifstream file(filepath, ifstream::binary);

	if (!file.is_open()) error("Não foi possível abrir o arquivo '" + filename + "'.");

	int n = 0; // nodes
	int i = 0, i1 = 0;
	int a = 0, b = 0; // edge vertices
	string buffer;
	bool line0 = true;
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
				this->n = n;
			}
		}

		else { // "p 196 197"
			a = stoi(buffer);
			i0 = i;

			buffer = line.substr(i0+1, line.length()-i0);
			b = stoi(buffer);

			// convert 1-indexed vertices to 0-indexed
			if (!a || !b) error("SHIT");
			a = a-1;
			b = b-1;

			// add edge to edges list
			INT_PAIR edge = make_pair(a, b);
			this->edges.push_back(edge);
		}

		line0 = false;
	} // while

	file.close();

	return;
} // make_graph

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

int Graph::degree(int vertex) {
	int degree = 0;
	for (auto it = edges.begin(); it != edges.end(); it++) {
		if (it->first == vertex || it->second == vertex) degree++;
	}

	return degree;
}

INT_LIST Graph::get_vertex_list(void) {
	INT_SET vertices_set = {};

	for (auto it = this->edges.begin(); it != this->edges.end(); it++) {
		vertices_set.insert(it->first);
		vertices_set.insert(it->second);
	}

	INT_LIST vertices(vertices_set.begin(), vertices_set.end());
	return vertices;
}

/*
functions to get the minimum and maximum degree values for the vertices in the graph
*/
int Graph::min_degree(void) {
	int min_degree = INT_MAX, degree;


	for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
		degree = this->degree(*it); // vertex degree

		if (degree < min_degree) min_degree = degree;
	}

	return min_degree;
}

int Graph::max_degree(void) {
	int max_degree = 0, degree;

	for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
		degree = this->degree(*it);

		if (degree > max_degree) max_degree = degree;
	}

	return max_degree;
}


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