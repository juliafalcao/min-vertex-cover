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

/*
Graph class declaration
*/
class Graph {
	
private:
	string name; // known name of test instance
	bool directed; // directed or undirected graph
	int n; // number of vertices (numbered from 0 to n-1)
	int m; // number of edges
	string file_suffix_to_remove = ".txt";

public:
	INT_PAIR_LIST edges; // list of edges
	INT_LIST vertices;

	Graph(string path, string filename, bool directed=false);
	void print_adj(void);
	void print_edges(void);
	void make_graph(string path, string filename);
	INT_PAIR_LIST get_edges_copy(void);
	int get_n(void);
	int get_m(void);
	string get_name(void);
	INT_LIST get_vertex_adj(int vertex);
	int degree(int vertex);
	INT_LIST get_vertex_list(void);
	int min_degree(void);
	int max_degree(void);
	
	INT_LIST sort_vertices_by_higher_degree(INT_LIST vertices);
	bool compare_degree_reverse(int v1, int v2);

	Graph(INT_SET test_vertices);
};


#endif