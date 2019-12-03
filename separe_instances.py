import pandas as pd


df: pd.DataFrame = pd.read_csv("results/base.csv")
df = df[["instance", "N"]]

unique_sizes = set(df["N"])
unique_sizes = sorted(list(unique_sizes))
unique_sizes = [size for size in unique_sizes if size < 2500]
test_filenames = []

# STR_LIST problems = {"C2000.9", "C2000.9", "C4000.5", "MANN_a45", "MANN_a81", "MANN_a81", "keller6", "p_hat1500-1", "p_hat1500-1", "san400_0.7_3"};

"""
select a random instance of each unique size to add to test set
"""
for size in unique_sizes:
	selection = df[df["N"] == size].sample(1)
	assert len(selection) > 0
	
	instance = list(selection["instance"])[0]

	filename = instance + ".clq-compliment.txt"
	test_filenames.append(filename)

with open("test_filenames.txt", mode="w", encoding="utf8") as file:
	for filename in test_filenames:
		file.write(filename + "\n")



