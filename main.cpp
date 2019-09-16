#include "main.h"

int main(void) {
	vector<string> filenames = list_dir("test_data/clique_complements/");
	int n_graphs = filenames.size();
	int n_test = floor(0.2 * n_graphs); // separate 20% for testing
	int n_train = n_graphs - n_test;
	cout << "Separated " << n_train << " graphs for training and " << n_test << " graphs for validation." << endl;

	return 0;
}