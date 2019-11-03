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
#include <math.h>

/* namespaces */
using namespace std;
using namespace std::chrono;

/* constants */
#define TIMESTAMP time_point<system_clock>
#define INT_SET_NULL INT_SET{-1} // to be used when errors happen in functions that return INT_SET
#define INT_MAX 2147483647

/* custom types */
typedef vector<int> INT_LIST;
typedef vector<string> STR_LIST;
typedef pair<int, INT_LIST> INT_LIST_PAIR;
typedef vector<INT_LIST_PAIR> ADJ_PAIR_LIST;
typedef pair<int,int> INT_PAIR;
typedef vector<INT_PAIR> INT_PAIR_LIST;
typedef set<int> INT_SET;

/* helpers */
void error(string message);
int find(string str, string ch, int start=0);
vector<string> list_dir(const char *path);
TIMESTAMP time();
long elapsed_time(time_point<system_clock> since);

/* structure functions */
bool most_values_comparator(const INT_LIST_PAIR &a, const INT_LIST_PAIR &b);
INT_LIST subtract(INT_LIST A, INT_LIST B);
int count_occurrences(INT_LIST V, int x);

/* printers */
void print_adj(ADJ_PAIR_LIST adj);
void print_int_set(INT_SET V);
void print_int_pair_list(INT_PAIR_LIST V);
void print_int_list(INT_LIST V);

#endif