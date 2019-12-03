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
#define BINARY_LIST_NULL BINARY_LIST{-1}
#define SOLUTIONS_LIST_NULL SOLUTIONS_LIST{BINARY_LIST_NULL}
#define INT_MAX 2147483647

/* custom types */
typedef vector<int> INT_LIST;
typedef vector<float> FLOAT_LIST;
typedef vector<string> STR_LIST;
typedef pair<int,int> INT_PAIR;
typedef vector<INT_PAIR> INT_PAIR_LIST;
typedef set<int> INT_SET;
typedef vector<INT_SET> INT_SET_LIST;
typedef set<INT_SET> SOLUTION_SET;
typedef vector<int> BINARY_LIST;
typedef vector<BINARY_LIST> SOLUTIONS_LIST;
typedef set<BINARY_LIST> BINARY_LIST_SET;


/* helpers */
void error(string message);
int find(string str, string ch, int start=0);
vector<string> list_dir(const char *path);
TIMESTAMP time();
long elapsed_time(time_point<system_clock> since);

/* structure functions */
int count_occurrences(INT_LIST V, int x);
INT_SET copy_int_set(INT_SET V);
INT_SET subtraction(INT_SET A, INT_SET B);
INT_SET set_union(INT_SET A, INT_SET B);
INT_SET symmetric_difference(INT_SET A, INT_SET B);

/* printers */
void print(INT_SET V);
void print(INT_PAIR_LIST V);
void print(INT_LIST V);
void print_solutions(SOLUTION_SET solutions);

BINARY_LIST copy(BINARY_LIST A);

/* vector to string */
string str(FLOAT_LIST vec);
string str(INT_LIST vec);

/* random */
float randf(float min, float max);
int rng(int i);

#endif