import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

# df: pd.DataFrame = pd.read_csv("results/base.csv")
dfb: pd.DataFrame = pd.read_csv("results/base.csv")

dfb["valid"] = dfb["opt"] < dfb["N"]
dfb = dfb[dfb["valid"]] # remove invalid instances
dfb = dfb.drop(columns=["valid"])

"""
GRASP vs. GRASP-PR comparison
"""
df_g = pd.read_csv("results/grasp.csv")
df_pr = pd.read_csv("results/grasp-pr.csv") # [!]

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
# df = df.drop(labels=["alpha"], axis=1)

df["result"] = df.apply(lambda row: "better" if (row["grasp-pr"] < row["grasp"]) else "equal" if (row["grasp-pr"] == row["grasp"]) else "worse", axis=1)

df = df.merge(dfb[["instance", "opt", "N"]], how="inner", on="instance")
df = df[["instance", "N", "opt", "grasp", "t(grasp)", "grasp-pr", "t(grasp-pr)", "result"]]
df["t(grasp)"] = df["t(grasp)"].apply(lambda x: x*0.001)
df["t(grasp-pr)"] = df["t(grasp-pr)"].apply(lambda x: x*0.001)

df.to_csv("results/formatted_grasp.csv", index=False)

# line plot
df["grasp"] = df["opt"]/df["grasp"]
df["grasp"] = df["opt"]/df["grasp-pr"]
