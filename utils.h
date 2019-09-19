#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <chrono>
#include <set>
#include <algorithm>

/* namespaces */
using namespace std;
using namespace std::chrono;

/* constants */
#define TIMESTAMP time_point<system_clock>

/* custom types */
typedef vector<int> INT_LIST;
typedef vector<string> STR_LIST;
typedef pair<int, INT_LIST> INT_LIST_PAIR;
typedef vector<INT_LIST_PAIR> ADJ_PAIR_LIST;
typedef pair<int,int> INT_PAIR;
typedef vector<INT_PAIR> INT_PAIR_LIST;
typedef set<int> INT_SET;

/* methods */
void error(string message);
int find(string str, string ch, int start=0);
vector<string> list_dir(const char *path);
TIMESTAMP time();
long elapsed_time(time_point<system_clock> since);
bool most_values_comparator(const INT_LIST_PAIR &a, const INT_LIST_PAIR &b);
INT_LIST subtract(INT_LIST A, INT_LIST B);
void print_adj(ADJ_PAIR_LIST adj);


#endif