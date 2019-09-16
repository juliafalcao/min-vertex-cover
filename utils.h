#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

void error(string message);
int find(string str, string ch, int start = 0);
vector<string> list_dir(const char *path);

#endif