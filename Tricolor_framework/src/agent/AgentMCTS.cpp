#include "AgentMCTS.h"
#include <algorithm>
#include <cmath>
#include <iostream>

AgentMCTS::AgentMCTS(int sim, double exploration)
    : Agent(true), simulations(sim), exploration_constant(exploration){
    std::random_device rd;
    rng = std::mt19937(rd());
}

ActionOpti AgentMCTS::choose_action(BoardOpti& board, int player) {
    auto start = std::chrono::high_resolution_clock::now();
    auto deadline = start + std::chrono::milliseconds(1000);

    Node root(board, player);
    root.untried_actions = board.get_actions(player);

    int sim_count = 0;

    while (std::chrono::high_resolution_clock::now() < deadline) {
        Node* node = &root;

        // 1. Selection
        while (node->untried_actions.empty() && !node->children.empty()) {
            node = select(node);
        }

        // 2. Expansion
        if (!node->untried_actions.empty()) {
            node = expand(node);
        }

        // 3. Simulation
        double result = simulate(node, player);

        // 4. Backpropagation
        backpropagate(node, result);

        sim_count++;
    }

    // Pick the child with highest visit count
    auto best_child = std::max_element(root.children.begin(), root.children.end(),
        [](const std::unique_ptr<Node>& a, const std::unique_ptr<Node>& b) {
            return a->visits < b->visits;
        });

    if (best_child != root.children.end()) {
        return (*best_child)->action_from_parent;
    } else {
        // fallback if no children
        auto actions = board.get_actions(player);
        return actions[0];
    }
}

// UCT formula
AgentMCTS::Node* AgentMCTS::select(Node* node) {
    Node* best = nullptr;
    double best_uct = -1e9;

    for (auto& child : node->children) {
        // Force exploration of unvisited nodes
        if (child->visits == 0) {
            return child.get();
        }

        double exploitation = child->value / child->visits;
        double exploration = exploration_constant *
            std::sqrt(std::log(node->visits) / child->visits);

        double uct = exploitation + exploration;

        if (uct > best_uct) {
            best_uct = uct;
            best = child.get();
        }
    }
    return best;
}

AgentMCTS::Node* AgentMCTS::expand(Node* node) {
    std::uniform_int_distribution<size_t> dist(0, node->untried_actions.size() - 1);
    size_t index = dist(rng);
    ActionOpti action = node->untried_actions[index];

    BoardOpti next_state = node->state.result_state_after_action(node->player_to_move, action);
    auto new_node = std::make_unique<Node>(next_state, (node->player_to_move + 1) % 2, node, action); // store action
    new_node->untried_actions = next_state.get_actions(new_node->player_to_move);

    Node* ptr = new_node.get();
    node->children.push_back(std::move(new_node));
    node->untried_actions.erase(node->untried_actions.begin() + index);

    return ptr;
}

// Simple random playout
double AgentMCTS::simulate(Node* node, int player) {
    BoardOpti state = node->state;
    int current_player = node->player_to_move;

    while (true) {
        auto actions = state.get_actions(current_player);
        if (actions.empty()) {
            // If current player has no moves, opponent wins
            return current_player == player ? 0.0 : 1.0;
        }

        std::uniform_int_distribution<size_t> dist(0, actions.size() - 1);
        ActionOpti action = actions[dist(rng)];
        state = state.result_state_after_action(current_player, action);
        current_player = (current_player + 1) % 2;
    }
}

// Backpropagate result
void AgentMCTS::backpropagate(Node* node, double result) {
    while (node != nullptr) {
        node->visits++;
        node->value += result;
        node = node->parent;
    }
}