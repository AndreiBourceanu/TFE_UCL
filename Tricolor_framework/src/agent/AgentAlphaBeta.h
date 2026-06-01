#pragma once

#include "Agent.h"
#include <functional>
#include <chrono>
#include <vector>

// Alpha-Beta Agent class
class AgentAlphaBeta : public Agent {
public:
    // Heuristic function
    using Heuristic = std::function<int(BoardOpti&, int)>;

    // Constructor takes heuristic function
    AgentAlphaBeta(Heuristic h);

    // Alpha-Beta algorithm
    int alpha_beta(BoardOpti& board, int depth, int alpha, int beta,
                   int initial_player, bool is_max, int player,
                   std::chrono::high_resolution_clock::time_point deadline);

    ActionOpti choose_action(BoardOpti& board, int player) override;

private:
    Heuristic heuristic;
};