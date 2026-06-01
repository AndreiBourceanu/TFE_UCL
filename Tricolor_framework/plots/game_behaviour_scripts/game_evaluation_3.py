import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys
import os

args = sys.argv[1:]

df = pd.read_csv("../../csv_files/game_behaviour_" + args[0] + "_vs_" + args[1] + "/game_evaluation_3.csv")

y = df["game_evaluation_3"]
x = np.arange(len(y))

plt.figure(figsize=(10, 4))

ymin, ymax = -1, 1

# Fill below the curve (white)
plt.fill_between(x, ymin, y, color='white')

# Fill above the curve (black)
plt.fill_between(x, y, ymax, color='black')

# Draw the curve on top
#plt.plot(x, y, color='black', linewidth=1)

plt.axhline(0, color='yellow', linewidth=1)

plt.ylim(ymin, ymax)
plt.xlim(x.min(), x.max())

plt.xlabel("Turn")
plt.ylabel("Evaluation")
plt.title("Game 3 Evaluation" + "\n" + args[0] + " vs " + args[1])

plt.tight_layout()
os.makedirs("../game_behaviour" + "_" + args[0] + "_vs_" + args[1], exist_ok=True)
plt.savefig("../game_behaviour" + "_" + args[0] + "_vs_" + args[1] + "/game_evaluation_3.pdf")