CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = $(shell pkg-config --cflags --libs sfml-graphics)

SRC_HELPING_FILES = src/agent/Agent.cpp src/agent/AgentRandom.cpp src/agent/AgentHuman.cpp src/agent/AgentAlphaBeta.cpp src/agent/AgentMCTS.cpp src/agent/Heuristics.cpp src/board/Board.cpp src/game/Game.cpp src/board/Zobrist.cpp

SRC_GAME_BEHAVIOUR_METRICS = src/game_behaviour_metrics.cpp $(SRC_HELPING_FILES)

SRC_TRICOLOR_GAME = src/launch_game.cpp $(SRC_HELPING_FILES)
SRC_TRICOLOR_GAME_OPTI = src/launch_game_opti.cpp $(SRC_HELPING_FILES)

OUT_GAME_BEHAVIOUR_METRICS = game_behaviour_metrics

OUT_GAME = tricolor_game
OUT_GAME_OPTI = tricolor_game_opti

all:
	make game_behaviour_metrics
	make tricolor_game_opti
game_behaviour_metrics:
	$(CXX) $(SRC_GAME_BEHAVIOUR_METRICS) -o $(OUT_GAME_BEHAVIOUR_METRICS)
#tricolor_game:
#	$(CXX) $(SRC_TRICOLOR_GAME) -o $(OUT_GAME) $(LDFLAGS)
tricolor_game_opti:
	$(CXX) $(SRC_TRICOLOR_GAME_OPTI) -o $(OUT_GAME_OPTI) $(LDFLAGS)
clean:
	rm -f $(OUT_GAME) $(OUT_GAME_BEHAVIOUR_METRICS) $(OUT_GAME_OPTI)
