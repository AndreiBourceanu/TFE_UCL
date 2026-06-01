#include <iostream>
#include <random>
#include "AgentRandom.h"

using namespace std;


AgentRandom::AgentRandom() : Agent(true) {}

// Function for simulations: choose a random action
Action AgentRandom::choose_action(Board& board, vector<Action>& actions){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, actions.size() - 1);
    int x = dist(gen);
    return actions[x];
}

// Function for the UI: choose a random action
ActionOpti AgentRandom::choose_action(BoardOpti& board, int player){
    vector<ActionOpti> actions = board.get_actions(player);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, actions.size() - 1);
    int x = dist(gen);
    return actions[x];
}