#include "utils.h"

/*
aux function to handle errors that should print a message and stop the program
*/
void error(string message) {
	cout << message << endl;
	exit(1);
}

/*
aux function to find the index of a given character in a string, starting from a given point
*/
int find(string str, string ch, int start) {
	for (int i = start; i < str.length(); i++) {
		if (string(1, str[i]) == ch) {
			return i;
			break;
		}
	}

	return -1;
}

/*
list files in given directory
https://github.com/tronkko/dirent
https://www.tutorialspoint.com/How-can-I-get-the-list-of-files-in-a-directory-using-C-Cplusplus
*/
vector<string> list_dir(const char *path) {
	vector<string> filenames;
	struct dirent *entry;
	DIR *dir = opendir(path);

	if (dir == NULL) {
		error("Diretório nulo.");
	}
	
	while ((entry = readdir(dir)) != NULL) {
		char txt[5] = ".txt";
		if (strstr(entry->d_name, txt)) { // to include only filenames that contain ".txt"
			filenames.push_back(entry->d_name);
		}
	}
	
	closedir(dir);
	return filenames;
}

/*
function wrappers for time functionalities
*/

TIMESTAMP time() {
	/* returns current point in time (to be used for t0) */
	return high_resolution_clock::now();
}

long elapsed_time(TIMESTAMP since) {
	/* returns elapsed time since given timestamp in milliseconds (to be used for t-t0) */
	long dt = duration_cast<milliseconds>(time() - since).count();
	return dt;
}

/*
print set of integers as { 1 2 3 }
*/
void print(INT_SET V) {
	printf("{ ");
	for (auto it = V.begin(); it != V.end(); it++) printf("%d ", *it);
	printf("}\n");
}

/*
print vector of integers as { 1 2 3 }
*/
void print(INT_LIST V) {
	printf("{ ");
	for (auto it = V.begin(); it != V.end(); it++) printf("%d ", *it);
	printf("}\n");
}

/*
print list of int pairs as { (1, 2) (3, 4) (5, 6) }
*/
void print(INT_PAIR_LIST V) {
	printf("{ ");
	for (auto it = V.begin(); it != V.end(); it++) printf("(%d, %d) ", it->first, it->second);
	printf("}\n");
}

BINARY_LIST copy(BINARY_LIST A) {
	BINARY_LIST B(A.size());
	for (int i = 0; i < A.size(); i++) B[i] = A[i];

	return B;
}

/*
count occurrences of x in list A
*/
int count_occurrences(INT_LIST V, int x) {
	int count = 0;

	for (auto it = V.begin(); it != V.end(); it++) {
		if (*it == x) count++;
	}

	return count;
}

string str(FLOAT_LIST vec) {
	string str = "";

	for (auto it = vec.begin(); it != vec.end(); it++) {
		str = str + to_string(*it) + " ";
	}

	return str;
}

string str(INT_LIST vec) {
	string str = "";

	for (auto it = vec.begin(); it != vec.end(); it++) {
		str = str + to_string(*it) + " ";
	}

	return str;
}

/*
deepcopy of set<int>
*/
INT_SET copy_int_set(INT_SET V) {
	INT_SET C;
	for (auto it = V.begin(); it != V.end(); it++) C.insert(*it);

	return C;
}

/*
given a set of solutions, print each of their sizes
*/
void print_solutions(SOLUTION_SET solutions) {
	INT_SET solution;
	int i = 0;

	for (auto it = solutions.begin(); it != solutions.end(); it++) {
		solution = *it;
		printf("Solution #%d: size %d\n", i, solution.size());
		i++;
	}
}

/*
set subtraction
returns elements that are in A but not in B
given A = {0,1,2} and B = {1,2,3,4}, then A-B = {0} and B-A = {3,4}
*/
INT_SET subtraction(INT_SET A, INT_SET B) {
	INT_SET subtraction = {};
	for (auto it = A.begin(); it != A.end(); it++) {
		if (find(B.begin(), B.end(), *it) == B.end()) {
			subtraction.insert(*it);
		}
	}

	return subtraction;
}

INT_SET set_intersection(INT_SET A, INT_SET B) {
	INT_SET intersection = {};
	for (auto it = A.begin(); it != A.end(); it++) {
		if (find(B.begin(), B.end(), *it) != B.end()) { // if element is in B too
			intersection.insert(*it);
		}
	}

	return intersection;
}

INT_SET set_union(INT_SET A, INT_SET B) {
	for (auto it = B.begin(); it != B.end(); it++) {
		A.insert(*it);
	}

	return A;
}

/* disjunctive union
*/
INT_SET symmetric_difference(INT_SET A, INT_SET B) {
	return set_union(subtraction(A, B), subtraction(B, A));
}


float randf(float min, float max) {
	return  (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min;
}

int rng(int i) {
	return rand() % i;
}