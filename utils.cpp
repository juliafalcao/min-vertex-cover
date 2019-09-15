#include "utils.h"

/*
aux function to handle errors that should stop the program
*/
void error(string message) {
	cout << message << endl;
	exit(1);
}

/*
aux function to find the index of a given character in a string, starting from a given point
*/
int find(string str, string ch, int start = 0) {
	for (int i = start; i < str.length(); i++) {
		if (string(1, str[i]) == ch) {
			return i;
			break;
		}
	}

	return -1;
}