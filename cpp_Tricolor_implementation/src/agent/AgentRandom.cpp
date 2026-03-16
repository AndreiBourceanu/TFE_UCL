#include <iostream>
#include <random>
#include "AgentRandom.h"

using namespace std;


AgentRandom::AgentRandom() : Agent(true) {}

Action AgentRandom::choose_action(Board& board, vector<Action>& actions){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, actions.size() - 1);
    int x = dist(gen);
    return actions[x];
}

ActionOpti AgentRandom::choose_action(BoardOpti& board, vector<ActionOpti>& actions){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, actions.size() - 1);
    int x = dist(gen);
    return actions[x];
}