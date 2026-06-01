import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys
import os

args = sys.argv[1:]

path1 = "../../csv_files/game_behaviour_" + args[0] + "_vs_" + args[1] + "/winrates.csv"
df = pd.read_csv(path1)

if args[0] != args[1]:
    path2 = "../../csv_files/game_behaviour_" + args[1] + "_vs_" + args[0] + "/winrates.csv"
    df2 = pd.read_csv(path2)
    df2[["p1", "p2"]] = df2[["p2", "p1"]]
    
    # sum values
    df = df.add(df2, fill_value=0)

p1, p2, draw = df["p1"][0], df["p2"][0], df["draw"][0]
total_games = p1 + p2 + draw
p1 = p1 * 100 / total_games
p2 = p2 * 100 / total_games
draw = draw * 100 / total_games

labels = ["Player 1 (White)", "Player 2 (Black)", "Draw"]
if args[0] != args[1]:
    labels = [args[0], args[1], "Draw"]
values = [p1, p2, draw]

bars = plt.bar(labels, values)
plt.ylabel("Percentage (%)")
plt.title("Win percentages (%) of players" + "\n" + args[0] + " vs " + args[1])

for bar in bars:
    height = bar.get_height()
    plt.text(
        bar.get_x() + bar.get_width() / 2,
        height,
        f"{height:.1f}%",
        ha='center',
        va='bottom'
    )

os.makedirs("../game_behaviour" + "_" + args[0] + "_vs_" + args[1], exist_ok=True)
plt.savefig("../game_behaviour" + "_" + args[0] + "_vs_" + args[1] + "/winrates.pdf")