#include "utils.h"

/*
aux function to handle errors that should print a message and stop the program
*/
void error(string message) {
	cerr << message << endl;
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
void print_adj(ADJ_PAIR_LIST adj) {
	for (auto it = adj.begin(); it != adj.end(); it++) {
		cout << it->first << ": { ";
		
		for (auto v_it = it->second.begin(); v_it != it->second.end(); v_it++) {
			cout << *v_it << " ";
		}

		cout << "}" << endl;
	}
}

void print_int_set(INT_SET V) {
	printf("{ ");
	for (auto it = V.begin(); it != V.end(); it++) printf("%d ", *it);
	printf("}\n");
}

bool equal_pairs(INT_PAIR p1, INT_PAIR p2) {
	return (p1.first == p2.first && p1.second == p2.second) || (p1.first == p2.second && p1.second == p2.first);
}

void print_int_pair_list(INT_PAIR_LIST V) {
	printf("{ ");
	for (auto it = V.begin(); it != V.end(); it++) printf("(%d, %d) ", it->first, it->second);
	printf("}\n");
}
