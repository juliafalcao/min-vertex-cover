import pandas as pd

# df: pd.DataFrame = pd.read_csv("results/base.csv")
df: pd.DataFrame = pd.read_csv("results/base.csv")

df["valid"] = df["opt"] < df["N"]
df = df[df["valid"]] # remove invalid instances
df = df.drop(columns=["valid"])

""" greedy and rm-greedy
df_greedy: pd.DataFrame = pd.read_csv("results/greedy.csv")
df_rmgreedy: pd.DataFrame = pd.read_csv("results/rm-greedy.csv")

# set "instance" as indexes to make sure there's only one row per instance
df = df.set_index("instance")
df_greedy = df_greedy.set_index("instance")
df_rmgreedy = df_rmgreedy.set_index("instance")

# join
df["greedy"] = df_greedy["greedy"]
df["t(greedy)"] = df_greedy["runtime"]
df["rm-greedy"] = df_rmgreedy["rm-greedy"]
df["t(rm-greedy)"] = df_rmgreedy["runtime"]

# remove instances not in test_data
df = df[df["greedy"].notnull()]
df = df[df["rm-greedy"].notnull()]

# format properly for export
df["greedy"] = pd.to_numeric(df["greedy"], downcast="integer")
df["rm-greedy"] = pd.to_numeric(df["rm-greedy"], downcast="integer")

# export
df.to_csv("results/formatted_results.csv", index_label="instance")
"""

df_ls: pd.DataFrame = pd.read_csv("results/ls-first.csv")

# list of instances to increase max_it_first_iterations and run again
"""
to_redo = df_ls[df_ls["search-iterations"] == 0]["instance"]
to_redo = [instance + ".clq-compliment.txt" for instance in to_redo]
print("{", end="");
for filename in to_redo:
	print(f'"{filename}", ', end="")
print("}")
"""

df_ls.set_index("instance")

# join
df["ls-first"] = df_ls["mvc"]
df["t(ls-first)"] = df_ls["runtime"]

# format
df["ls-first"] = pd.to_numeric(df["ls-first"], downcast="integer")

# export
df.to_csv("results/formatted_results.csv", index_label="instance")