#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define TIMESTAMP time_point<system_clock>

void error(string message);
int find(string str, string ch, int start = 0);
vector<string> list_dir(const char *path);
TIMESTAMP time();
long elapsed_time(time_point<system_clock> since);


#endif