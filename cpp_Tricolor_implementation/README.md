# This directory contains my C++ implementation of Tricolor

## Setup

### Linux

First, install essentials (if not already done) to have `g++` and `make`:
<pre>
sudo apt update 
sudo apt install build-essential
</pre>

Then, install the SFML library for 2D graphics:
<pre>
sudo apt install libsfml-dev
</pre>

## How to run

### Game experiments

The following command builds the executable that runs the experiments. I simulate `10.000` Tricolor games between 2 `random` agents and then compute the:
- Total time taken to simulate all games (about `47 seconds` on my machine)
- Number of games simulated in 1 second on average (about `209` on my machine)
- Number of turns played in all games in total (about `1.776.775` on my machine)
- Number of turns played in 1 second on average (about `37253` on my machine)
- Number of times white wins, which is the first player to make a move (`5002` from last experiment)
- Number of times black wins (`4891` from last experiment)
- Number of draws (`107` from last experiment)
<pre>
make random_game_measures
</pre>
then, to run it (it takes about 47 seconds on my machine, as I said):
<pre>
./random_game_measures
</pre>

### Playing the game

The following command builds the executable that runs a game of Tricolor between 2 players:
<pre>
make tricolor_game
</pre>
then, to run it:
<pre>
./tricolor_game "agent1" "agent2"
</pre>
like for example
<pre>
./tricolor_game human random
</pre>

Then, a window will pop up and the game starts immediatly.

## How to play the game

### Rules

The rules can be red in the `Boutin - Tricolor, 2021.pdf` file in the `books/` directory found at the root.

### Board and Pieces interaction

As a human, you will see your `available` moves by the `blue circles` on the corresponding hex tiles. This means you can only move pieces and stacks on those given hex tiles.

To select different number of pieces from a stack, use `left-click` to `remove` one piece and `right-click` to `add` one. The number of pieces you `selected` is colored in `green`. By default, the first time when you click on a stack, all the pieces will be selected. If you wish to unselect a stack, click outside the board or on a hex tile where you can't move.

### "50-moves rule"

If no `imprisonment` or `capture` happened in the last `50` moves, then the game ends in a `draw`. This rule was added by me but can be modified.