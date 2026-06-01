import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys
import os

args = sys.argv[1:]

path1 = "../../csv_files/game_behaviour_" + args[0] + "_vs_" + args[1] + "/total_branches.csv"
df = pd.read_csv(path1)

if args[0] != args[1]:
    path2 = "../../csv_files/game_behaviour_" + args[1] + "_vs_" + args[0] + "/total_branches.csv"
    df2 = pd.read_csv(path2)
    
    df = pd.concat([df, df2], ignore_index=True)

x = df["total_branches"]

plt.violinplot(x)

plt.title("Average branching factor during a game" + "\n" + args[0] + " vs " + args[1])
plt.ylabel("Average branching factor")
mean = np.mean(x)
std = np.std(x)
plt.errorbar(1, mean, yerr=std, fmt='o', color='red', capsize=5, label='Mean ± SD')
plt.text(1.1, mean, f"Mean: {mean:.2f}\nSD: {std:.2f}", va='center')
plt.gca().set_xticks([])

plt.legend()
os.makedirs("../game_behaviour" + "_" + args[0] + "_vs_" + args[1], exist_ok=True)
plt.savefig("../game_behaviour" + "_" + args[0] + "_vs_" + args[1] + "/total_branches.pdf")