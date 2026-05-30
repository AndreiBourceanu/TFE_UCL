import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df = pd.read_csv("../../csv_files/game_behaviour/game_evaluation_1.csv")

y = df["game_evaluation_1"]
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
plt.title("Game 1 Evaluation - White wins")

plt.tight_layout()
plt.savefig("../game_behaviour/game_evaluation_1.pdf")