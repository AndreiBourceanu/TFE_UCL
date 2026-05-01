import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df = pd.read_csv("../../csv_files/game_behaviour/winrates.csv")

p1, p2, draw = df["p1"][0], df["p2"][0], df["draw"][0]
total_games = p1 + p2 + draw
p1 = p1 * 100 / total_games
p2 = p2 * 100 / total_games
draw = draw * 100 / total_games

labels = ["Player 1 (White)", "Player 2 (Black)", "Draw"]
values = [p1, p2, draw]

bars = plt.bar(labels, values)
plt.ylabel("Percentage (%)")
plt.title("Win percentages (%) of players")

for bar in bars:
    height = bar.get_height()
    plt.text(
        bar.get_x() + bar.get_width() / 2,
        height,
        f"{height:.1f}%",
        ha='center',
        va='bottom'
    )

plt.savefig("../game_behaviour/winrates.pdf")