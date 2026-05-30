#pragma once

#include "Agent.h"
#include <chrono>
#include <vector>
#include <memory>
#include <random>

class AgentMCTS : public Agent {
public:
    AgentMCTS(int simulations, double exploration = 1.41);

    ActionOpti choose_action(BoardOpti& board, int player) override;

private:
    struct Node {
        BoardOpti state;
        int player_to_move;
        Node* parent;
        std::vector<std::unique_ptr<Node>> children;
        std::vector<ActionOpti> untried_actions;

        ActionOpti action_from_parent;
        int visits = 0;
        double value = 0.0;

        Node(BoardOpti s, int p, Node* par = nullptr, ActionOpti a = ActionOpti())
            : state(s), player_to_move(p), parent(par), action_from_parent(a) {}
    };

    int simulations;
    double exploration_constant;
    std::mt19937 rng;

    Node* select(Node* node);
    Node* expand(Node* node);
    double simulate(Node* node, int player);
    void backpropagate(Node* node, double result);
};