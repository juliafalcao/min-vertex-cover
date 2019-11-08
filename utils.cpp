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
comparator to use for sorting
*/
bool most_values_comparator(const INT_LIST_PAIR &a, const INT_LIST_PAIR &b) {
	return a.second.size() != b.second.size() ? a.second.size() > b.second.size() : a.first < b.first;
}

/*
set operation A-B, removes from A all elements present in B and returns new A
*/
INT_LIST subtract(INT_LIST &A, INT_LIST &B) {
	INT_LIST new_A = {};

	for (auto it = A.begin(); it != A.end(); it++) {
		if (find(B.begin(), B.end(), *it) == B.end()) {
			new_A.push_back(*it); // add to new_a elements not in b
		}
	}

	return new_A;
}

/*
print any ADJ_PAIR_LIST
*/
void print(ADJ_PAIR_LIST adj) {
	for (auto it = adj.begin(); it != adj.end(); it++) {
		cout << it->first << ": { ";
		
		for (auto v_it = it->second.begin(); v_it != it->second.end(); v_it++) {
			cout << *v_it << " ";
		}

		cout << "}" << endl;
	}
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

INT_SET copy_int_set(INT_SET V) {
	INT_SET C;
	for (auto it = V.begin(); it != V.end(); it++) C.insert(*it);

	return C;
}

void print_solutions(set<INT_SET> solutions) {
	INT_SET solution;
	int i = 0;

	for (auto it = solutions.begin(); it != solutions.end(); it++) {
		solution = *it;
		printf("Solution #%d: size %d\n", i, solution.size());
		i++;
	}
}