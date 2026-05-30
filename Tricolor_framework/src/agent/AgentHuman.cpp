#include <iostream>
#include "AgentHuman.h"

using namespace std;

AgentHuman::AgentHuman() : Agent(false) {}

Action AgentHuman::choose_action(Board& board, vector<Action>& actions){
    return actions[0];
}

ActionOpti AgentHuman::choose_action(BoardOpti& board, vector<ActionOpti>& actions){
    return actions[0];
}

ActionOpti AgentHuman::choose_action(BoardOpti& board, int player){
    vector<ActionOpti> actions = board.get_actions(player);
    return actions[0];
}