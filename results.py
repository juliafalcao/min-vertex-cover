import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# df: pd.DataFrame = pd.read_csv("results/base.csv")
df: pd.DataFrame = pd.read_csv("results/base.csv")

df["valid"] = df["opt"] < df["N"]
df = df[df["valid"]] # remove invalid instances
df = df.drop(columns=["valid"])

"""
grasp alpha comparison
"""

df_alpha = pd.read_csv("results/grasp-tests.csv")
df_alpha = df_alpha.sort_values(by=["instance", "mvc_size"], ascending=[True, True])
df_alpha = df_alpha.drop_duplicates(subset=["instance"], keep="first")


labels, counts = np.unique(df_alpha["alpha"], return_counts=True)
plt.bar(labels, counts, align='center')
plt.xticks(labels)
plt.show()