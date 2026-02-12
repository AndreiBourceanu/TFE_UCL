#include <iostream>
#include "AgentHuman.h"

using namespace std;

AgentHuman::AgentHuman() : Agent(false) {}

Action AgentHuman::choose_action(Board& board, vector<Action>& actions){
    return actions[0];
}