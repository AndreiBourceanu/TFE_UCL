CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = $(shell pkg-config --cflags --libs sfml-graphics)

SRC_HELPING_FILES = src/agent/Agent.cpp src/agent/AgentRandom.cpp src/agent/AgentHuman.cpp src/board/Board.cpp src/game/Game.cpp
SRC_RANDOM_EXPERIMENT = src/random_experiment.cpp $(SRC_HELPING_FILES)
SRC_TRICOLOR_GAME = src/launch_game.cpp $(SRC_HELPING_FILES)

OUT_EXPERIMENT = random_experiment
OUT_GAME = tricolor_game

all:
	make random_game
	make tricolor_game
random_game:
	$(CXX) $(SRC_RANDOM_EXPERIMENT) -o $(OUT_EXPERIMENT)
tricolor_game:
	$(CXX) $(SRC_TRICOLOR_GAME) -o $(OUT_GAME) $(LDFLAGS)
clean:
	rm -f $(OUT_EXPERIMENT) $(OUT_GAME)
