import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df = pd.read_csv("../../csv_files/game_behaviour/total_repeated_states.csv")

x = df["total_repeated_states"]

plt.violinplot(x)

plt.title("Number of repeated states in a game")
plt.ylabel("Repeated states")
mean = np.mean(x)
std = np.std(x)
plt.errorbar(1, mean, yerr=std, fmt='o', color='red', capsize=5, label='Mean ± SD')
plt.text(1.1, mean, f"Mean: {mean:.2f}\nSD: {std:.2f}", va='center')
plt.gca().set_xticks([])

plt.legend()
plt.savefig("../game_behaviour/total_repeated_states.pdf")