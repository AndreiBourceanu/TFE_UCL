#pragma once
#include "Agent.h"

class AgentRandom : public Agent{
public:
AgentRandom();

Action choose_action(Board& board, vector<Action>& actions) override;
private:
};