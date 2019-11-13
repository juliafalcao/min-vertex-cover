import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm

# df: pd.DataFrame = pd.read_csv("results/base.csv")
df: pd.DataFrame = pd.read_csv("results/base.csv")

df["valid"] = df["opt"] < df["N"]
df = df[df["valid"]] # remove invalid instances
df = df.drop(columns=["valid"])

"""
grasp alpha comparison
"""

# df_alpha = pd.read_csv("results/grasp-tests.csv")
# df_alpha = df_alpha.sort_values(by=["instance", "mvc_size"], ascending=[True, True])
# df_alpha = df_alpha.drop_duplicates(subset=["instance"], keep="first")


# labels, counts = np.unique(df_alpha["alpha"], return_counts=True)
# labels = ["α = " + str(label) for label in labels]
# # counts = [count/25 for count in counts]

# fig, ax = plt.subplots(1, 1)
# ax.bar(labels, counts, align='center', width=0.5, color=[cm.viridis(i/5) for i in range(5)][1:4])
# ax.grid(True, axis="y", zorder=1000)
# ax.set_axisbelow(True)
# ax.set_ylabel("Nº de melhores resultados")
# ax.set_xlabel("Valores de alfa")
# plt.show()

"""
GRASP vs. GRASP-PR comparison
"""
df_g = pd.read_csv("results/grasp.csv")
df_pr = pd.read_csv("results/grasp-pr-prelim.csv") # [!]

# organize dfs
df_g = df_g[df_g["alpha"] == 0.3]
df_g = df_g.rename(columns={"mvc_size": "grasp", "runtime": "t(grasp)"})[["instance", "grasp", "t(grasp)"]]
df_g = df_g.sort_values(by=["instance", "grasp"], ascending=[True, True])
df_g = df_g.drop_duplicates(subset=["instance"], keep="first") # keep only the best result per instance

df_pr = df_pr[df_pr["alpha"] == 0.3]
df_pr = df_pr.rename(columns={"mvc_size": "grasp-pr", "runtime": "t(grasp-pr)"})[["instance", "grasp-pr", "t(grasp-pr)"]]
df_pr = df_pr.sort_values(by=["instance", "grasp-pr"], ascending=[True, True])
df_pr = df_pr.drop_duplicates(subset=["instance"], keep="first") # keep only the best result per instance

df = df_g.merge(df_pr, how="inner", on="instance")
df = df.drop(labels=["alpha"], axis=1)

df["result"] = df.apply(lambda row: "better" if (row["grasp-pr"] < row["grasp"]) else "equal" if (row["grasp-pr"] == row["grasp"]) else "worse", axis=1)
df.loc[df[df["grasp-pr"] > df["grasp"]], "result"] = "better"

df.to_csv("results/formatted_grasp.csv")