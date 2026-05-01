import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df = pd.read_csv("../../csv_files/game_behaviour/total_lead_change_moves.csv")

x = df["total_lead_change_moves"]

plt.violinplot(x)

plt.title("Average number of lead changing moves in a turn during a game")
plt.ylabel("Average number of lead changing moves in a turn")
mean = np.mean(x)
std = np.std(x)
plt.errorbar(1, mean, yerr=std, fmt='o', color='red', capsize=5, label='Mean ± SD')
plt.text(1.1, mean, f"Mean: {mean:.2f}\nSD: {std:.2f}", va='center')
plt.gca().set_xticks([])

plt.legend()
plt.savefig("../game_behaviour/total_lead_change_moves.pdf")