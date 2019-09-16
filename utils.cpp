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
		cout << entry->d_name << endl;
		filenames.push_back(entry->d_name);
	}
	
	closedir(dir);
	return filenames;
}