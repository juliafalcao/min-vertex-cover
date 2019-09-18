#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "utils.h"
#include "graph.h"

using namespace std;

bool verify_vertex_cover(Graph &g, INT_SET cover);
void most_neighbors_first(Graph &g);


#endif