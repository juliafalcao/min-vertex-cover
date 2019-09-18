#ifndef GRAPH_H
#define GRAPH_H

#include "utils.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>

using namespace std;

// #define BUILD_ADJ_LIST 1
// #define BUILD_EDGES_LIST 2
// #define BUILD_BOTH 3


/*
Graph class declaration
*/
class Graph {
	
private:
	string name; // known name of test instance
	bool directed; // directed or undirected graph
	int n; // number of vertices (numbered from 1 to n)
	int m; // number of edges
	string file_suffix_to_remove = ".clq-compliment.txt";

public:
	ADJ_PAIR_LIST adj; // pairs of vertices and lists of the vertices they're connected to
	INT_PAIR_LIST edges; // list of edges

	Graph(string path, string filename, bool directed=false);
	void print_adj();
	void print_edges();
	void make_graph(string path, string filename, int &n_vertices);
	void sort_adj_by_most_neighbors(void);
	ADJ_PAIR_LIST get_adj_copy();
	int get_m();
};


#endif