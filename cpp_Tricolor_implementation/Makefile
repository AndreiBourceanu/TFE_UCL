CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = $(shell pkg-config --cflags --libs sfml-graphics)

SRC_HELPING_FILES = src/agent/Agent.cpp src/agent/AgentRandom.cpp src/agent/AgentHuman.cpp src/agent/AgentAlphaBeta.cpp src/agent/AgentMCTS.cpp src/agent/Heuristics.cpp src/board/Board.cpp src/game/Game.cpp src/board/Zobrist.cpp

SRC_RANDOM_EXPERIMENT = src/random_experiment.cpp $(SRC_HELPING_FILES)
SRC_RANDOM_EXPERIMENT_OPTI = src/random_experiment_opti.cpp $(SRC_HELPING_FILES)
SRC_ALPHABETA_VS_RANDOM_SIMULATIONS = src/alphabeta_vs_random_simulations.cpp $(SRC_HELPING_FILES)

SRC_GAME_BEHAVIOUR_METRICS = src/game_behaviour_metrics.cpp $(SRC_HELPING_FILES)

SRC_TRICOLOR_GAME = src/launch_game.cpp $(SRC_HELPING_FILES)
SRC_TRICOLOR_GAME_OPTI = src/launch_game_opti.cpp $(SRC_HELPING_FILES)

OUT_EXPERIMENT = random_game_measures
OUT_EXPERIMENT_OPTI = random_game_measures_opti
OUT_ALPHABETA_VS_RANDOM_SIMULATIONS = alphabeta_vs_random_simulations
OUT_GAME_BEHAVIOUR_METRICS = game_behaviour_metrics

OUT_GAME = tricolor_game
OUT_GAME_OPTI = tricolor_game_opti

all:
	make random_game_measures
	make random_game_measures_opti
	game_behaviour_metrics
	make tricolor_game
	make tricolor_game_opti
random_game_measures:
	$(CXX) $(SRC_RANDOM_EXPERIMENT) -o $(OUT_EXPERIMENT)
random_game_measures_opti:
	$(CXX) $(SRC_RANDOM_EXPERIMENT_OPTI) -o $(OUT_EXPERIMENT_OPTI)
alphabeta_vs_random_simulations:
	$(CXX) $(SRC_ALPHABETA_VS_RANDOM_SIMULATIONS) -o $(OUT_ALPHABETA_VS_RANDOM_SIMULATIONS)
game_behaviour_metrics:
	$(CXX) $(SRC_GAME_BEHAVIOUR_METRICS) -o $(OUT_GAME_BEHAVIOUR_METRICS)
tricolor_game:
	$(CXX) $(SRC_TRICOLOR_GAME) -o $(OUT_GAME) $(LDFLAGS)
tricolor_game_opti:
	$(CXX) $(SRC_TRICOLOR_GAME_OPTI) -o $(OUT_GAME_OPTI) $(LDFLAGS)
clean:
	rm -f $(OUT_EXPERIMENT) $(OUT_EXPERIMENT_OPTI) $(OUT_GAME)
