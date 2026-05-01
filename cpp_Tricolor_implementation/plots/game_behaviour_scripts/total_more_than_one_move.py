import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df = pd.read_csv("../../csv_files/game_behaviour/total_more_than_one_move.csv")

x = df["total_more_than_one_move"]

plt.violinplot(x)

plt.title("Percentage (%) of turns where more than one move\n was possible during a game")
plt.ylabel("Percentage (%)")
mean = np.mean(x)
std = np.std(x)
plt.errorbar(1, mean, yerr=std, fmt='o', color='red', capsize=5, label='Mean ± SD')
plt.text(1.1, mean, f"Mean: {mean:.2f}\nSD: {std:.2f}", va='center')
plt.gca().set_xticks([])

plt.legend()
plt.savefig("../game_behaviour/total_more_than_one_move.pdf")