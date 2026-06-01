#!/bin/bash

./game_behaviour_metrics random random 1000000
./game_behaviour_metrics random alphabeta 50
./game_behaviour_metrics alphabeta random 50
./game_behaviour_metrics random mcts 50
./game_behaviour_metrics mcts random 50
./game_behaviour_metrics alphabeta alphabeta 100
./game_behaviour_metrics mcts mcts 100
./game_behaviour_metrics alphabeta mcts 50
./game_behaviour_metrics mcts alphabeta 50