import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

dfb: pd.DataFrame = pd.read_csv("results/base.csv")

dfb["valid"] = dfb["opt"] < dfb["N"]
dfb = dfb[dfb["valid"]] # remove invalid instances
dfb = dfb.drop(columns=["valid"])

"""
Genetic Algorithm
"""

gdf = pd.read_csv("results/genetic-4.csv")

df = gdf.merge(dfb, how="inner", on="instance")
df = df[["instance","N", "population", "crossover", "mutation", "stability", "runtime", "opt", "result"]]
df["opt"] = df["opt"].apply(int)
df["runtime"] = df["runtime"].apply(lambda t: t*0.001)
df["runtime"] = np.around(df["runtime"], decimals=2)
df = df.sort_values(by=["instance", "result"], ascending=[True, True])
df = df.drop_duplicates(subset="instance", keep="first")
df = df.sort_values(by="N", ascending=True)
df = df[["instance", "N", "opt", "result", "runtime"]]
df.to_csv("genetic-final.csv", index=False)