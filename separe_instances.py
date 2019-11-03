import pandas as pd


df: pd.DataFrame = pd.read_csv("results/base.csv")
df = df[["instance", "N"]]

unique_sizes = set(df["N"])
test_filenames = []

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



