#pragma once
#include "Agent.h"

class AgentHuman : public Agent{
public:

AgentHuman();

Action choose_action(Board& board, vector<Action>& actions) override;

private:
};