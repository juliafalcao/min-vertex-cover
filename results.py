import pandas as pd

# df_comparison: pd.DataFrame = pd.read_csv("results/base.csv")
df_comparison: pd.DataFrame = pd.read_csv("base.csv")

df_comparison["valid"] = df_comparison["opt"] < df_comparison["N"]
df_comparison = df_comparison[df_comparison["valid"]] # remove invalid instances

df_greedy: pd.DataFrame = pd.read_csv("results/simple_greedy.csv")

df_comparison = df_comparison.set_index("instance")
df_greedy = df_greedy.set_index("instance")
df_comparison["greedy"] = df_greedy["greedy"]
df_comparison["t(greedy)"] = df_greedy["runtime"]