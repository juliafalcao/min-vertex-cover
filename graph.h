#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>

using namespace std;

/* custom types */
typedef vector<int> INT_LIST;
typedef pair<int, INT_LIST> INT_LIST_PAIR;
typedef vector<INT_LIST_PAIR> ADJ_PAIR_LIST; // vector of INT_LIST_PAIR
typedef pair<int,int> INT_PAIR;
typedef vector<INT_PAIR> INT_PAIR_LIST;

/* function declarations */
int find(string str, string ch, int start);
void error(string message);


class Graph {
	
private:
	string name;
	bool directed;
	ADJ_PAIR_LIST adj;
	string file_suffix_to_remove;

public:
	Graph(string path, string filename, bool directed=false);
	void print_adj();
	ADJ_PAIR_LIST make_graph(string path, string filename);


};


#endif